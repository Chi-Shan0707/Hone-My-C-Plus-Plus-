# Value 生命周期修复方案

## 遇到的 Bug

### Bug 1：临时变量销毁导致梯度链断裂

**现象**：loss 不下降，backward() 后权重的 grad 始终为 0。

**原因**：旧版本中 `operator+`、`operator*` 等运算符**按值返回** `Value` 对象。
例如：

```cpp
// 旧版本（有 bug）
Value operator+(const Value& other) const {
    global_value_pool.emplace_back();
    Value* result = &global_value_pool.back();
    result->children = {const_cast<Value*>(this), ...};
    return *result; // ← 返回 Value 副本，children 里的指针还指向 pool 里的对象
}
```

问题在于：调用者拿到的是一个**副本**，副本的 `children` 确实有效，但当这个副本再次被当作子节点加入下一个运算时，传入的是 `this`（副本的地址，位于栈上或另一个临时位置），不是 pool 里的稳定地址。随着表达式求值结束，栈上的临时 `Value` 被销毁，`children` 里的指针变为悬空指针，backward 时访问非法内存或读取垃圾梯度。

**另一个问题**：即使中间值放进了 `global_value_pool`，权重 `Value` 仍存储在 `Matrix.data`（`vector<vector<Value>>`）里。`vector` 扩容时会移动元素，导致之前保存的指针失效。

---

## 修复方案：双池 + 全指针化

### 核心改动

1. **两个内存池**（均为 `list<Value>`，`list` 的 push_back/emplace_back 保证已有元素的地址不变）：
   - `param_pool`：存放所有权重（WTE、WPE、各层权重矩阵）。**整个训练期间不清空**。
   - `graph_pool`：存放每次前向传播的中间计算节点。**每步训练结束后调用 `graph_pool.clear()` 清空**。

2. **`Value` 运算全部改为静态方法，返回 `Value*`**（指向 pool 中的稳定对象）：

```cpp
// 新版本（修复后）
static Value* add(Value* a, Value* b) {
    return make_new(a->data + b->data, {a, b}, {1.0, 1.0});
}
static Value* make_new(double d, const vector<Value*>& children = {}, ...) {
    graph_pool.emplace_back();       // 在 list 末尾原地构造，地址永不失效
    Value* v = &graph_pool.back();
    v->children = children;          // 存指针，不存值
    return v;
}
```

3. **`Vector` 改为存储 `Value*`**，`Matrix` 改为存储 `vector<vector<Value*>>`。

4. **`rand_matrix` 改为向 `param_pool` 写入**：

```cpp
Matrix rand_matrix(int rows, int cols) {
    ...
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            param_pool.emplace_back(dist(rng)); // 参数永久存活
            m.data[i][j] = &param_pool.back();
        }
    return m;
}
```

5. **训练循环改为**：
   - 每步清空 `graph_pool`（不清空 `param_pool`）
   - 反向传播时梯度正确累积到 `param_pool` 中的权重
   - SGD 更新权重后清零梯度

### 为什么选择双池而非 shared_ptr

- `shared_ptr` 引用计数开销大，训练时节点数量庞大，性能影响明显。
- 双池方案更简单直接：参数生命周期 = 程序运行期，计算图生命周期 = 一个训练步。两者职责清晰，不需要引用计数。

---

## 修复后的状态

- ✅ `Value` 对象全部在 `param_pool` 或 `graph_pool` 中，地址稳定
- ✅ `children` 指针永远指向 pool 中有效对象
- ✅ backward() 能正确将梯度传回权重
- ✅ 每步 `graph_pool.clear()` 释放内存，`param_pool` 中权重持续更新
- ✅ `Config.lr` 统一控制学习率，不再硬编码
