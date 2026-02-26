# MicroGPT C++ 实现 TODO 清单

## 项目结构
```
MicroGPT/
├── include/          # 头文件
│   ├── value.hpp
│   ├── matrix.hpp
│   ├── operators.hpp
│   ├── attention.hpp
│   ├── mlp.hpp
│   ├── gpt.hpp
│   ├── optimizer.hpp
│   ├── tokenizer.hpp
│   └── dataset.hpp
├── src/              # 实现文件
│   ├── value.cpp
│   ├── matrix.cpp
│   ├── operators.cpp
│   ├── attention.cpp
│   ├── mlp.cpp
│   ├── gpt.cpp
│   ├── optimizer.cpp
│   ├── tokenizer.cpp
│   ├── dataset.cpp
│   ├── train.cpp     # 训练主程序
│   └── inference.cpp # 推理主程序
├── tests/            # 单元测试
│   ├── test_value.cpp
│   ├── test_matrix.cpp
│   ├── test_operators.cpp
│   ├── test_attention.cpp
│   └── test_integration.cpp
├── CMakeLists.txt    # 构建配置
├── input.txt         # 训练数据
└── README.md
```

---

## 第一周：基础设施（核心优先级★★★★★）

### Day 1-2: Value 自动求导类

#### ☐ 1.1 基础 Value 类
- [ ] 定义 Value 类结构
  - [ ] `double m_data` - 存储数值
  - [ ] `double m_grad` - 存储梯度
  - [ ] `vector<Value*> m_children` - 子节点
  - [ ] `vector<double> m_local_grads` - 局部导数
  
- [ ] 实现构造函数
  - [ ] `Value(double data)` - 叶子节点
  - [ ] `Value(double data, children, local_grads)` - 计算节点

- [ ] 实现基础访问器
  - [ ] `double data() const`
  - [ ] `double grad() const`
  - [ ] `void set_data(double)`
  - [ ] `void zero_grad()`

#### ☐ 1.2 运算符重载
- [ ] 加法 `operator+`
  - [ ] Value + Value
  - [ ] Value + double
  - [ ] double + Value (全局函数)
  
- [ ] 乘法 `operator*`
  - [ ] Value * Value
  - [ ] Value * double
  - [ ] double * Value
  
- [ ] 减法 `operator-`
  - [ ] 实现为 `a + (-b)`
  
- [ ] 除法 `operator/`
  - [ ] 实现为 `a * b^(-1)`
  
- [ ] 负号 `operator-()` (unary)

#### ☐ 1.3 数学函数
- [ ] `Value pow(double n)` - 幂运算
- [ ] `Value exp()` - 指数
- [ ] `Value log()` - 对数
- [ ] `Value relu()` - ReLU 激活

#### ☐ 1.4 反向传播
- [ ] 实现 `backward()`
  - [ ] 拓扑排序 DFS
  - [ ] 初始化梯度为 1.0
  - [ ] 逆序累积梯度
  
- [ ] 测试反向传播
  - [ ] 简单图：c = a + b
  - [ ] 复杂图：c = (a + b) * (a - b)
  - [ ] 手算验证梯度

#### 📝 测试检查点
```cpp
// test_value.cpp
void test_simple_backward() {
    Value a(3.0);
    Value b(4.0);
    Value c = a * b;  // c = 12
    c.backward();
    // 期望：a.grad = 4, b.grad = 3
}
```

**预计时间：2天**  
**里程碑：能计算简单表达式的梯度**

---

### Day 3-4: Matrix 矩阵类和基础算子

#### ☐ 2.1 矩阵容器
- [ ] 定义 `Matrix` 类
  ```cpp
  class Matrix {
      vector<vector<Value>> data;
      size_t rows, cols;
  };
  ```
  
- [ ] 构造函数
  - [ ] `Matrix(rows, cols)` - 零初始化
  - [ ] `Matrix(rows, cols, mean, std)` - 高斯随机初始化
  - [ ] `Matrix(vector<vector<double>>)` - 从数组构造

- [ ] 访问器
  - [ ] `Value& at(i, j)`
  - [ ] `const Value& at(i, j) const`
  - [ ] `size_t rows() const`
  - [ ] `size_t cols() const`

- [ ] 定义 `Vector` 类型
  ```cpp
  using Vector = vector<Value>;
  ```

#### ☐ 2.2 线性代数运算
- [ ] `Vector matmul(const Matrix& W, const Vector& x)`
  - [ ] 实现矩阵-向量乘法
  - [ ] 检查维度匹配
  
- [ ] `Vector add(const Vector& a, const Vector& b)`
  - [ ] 逐元素相加
  
- [ ] `Vector scale(const Vector& v, double s)`
  - [ ] 标量乘法

#### ☐ 2.3 神经网络算子
- [ ] `Vector relu(const Vector& x)`
  - [ ] 逐元素 ReLU
  
- [ ] `Vector softmax(const Vector& logits)`
  - [ ] 数值稳定版：先减最大值
  - [ ] 指数、求和、归一化
  - [ ] 测试：和为 1
  
- [ ] `Vector rmsnorm(const Vector& x)`
  - [ ] 均方根归一化
  - [ ] 公式：x / sqrt(mean(x^2) + eps)

#### 📝 测试检查点
```cpp
void test_softmax() {
    Vector x = {Value(1.0), Value(2.0), Value(3.0)};
    Vector probs = softmax(x);
    
    // 验证和为 1
    double sum = 0;
    for (auto& p : probs) sum += p.data();
    assert(abs(sum - 1.0) < 1e-6);
}
```

**预计时间：2天**  
**里程碑：能进行矩阵运算和激活函数**

---

## 第二周：GPT 模型架构（优先级★★★★★）

### Day 5-6: Attention 模块

#### ☐ 3.1 单头 Attention
- [ ] 实现 `attention_head()` 函数
  ```cpp
  Vector attention_head(
      const Vector& q,      // query
      const vector<Vector>& keys,   // 历史 keys
      const vector<Vector>& values  // 历史 values
  );
  ```
  
- [ ] 步骤：
  - [ ] 计算 scores = q · k / sqrt(d)
  - [ ] Softmax 归一化
  - [ ] 加权求和 values
  
- [ ] 测试单头 attention
  - [ ] 2 个 token，手算验证
  - [ ] 打印中间 scores 和 weights

#### ☐ 3.2 多头 Attention
- [ ] 实现 `AttentionBlock` 类
  ```cpp
  class AttentionBlock {
      Matrix wq, wk, wv, wo;
      int n_head, head_dim;
      
      Vector forward(const Vector& x, 
                     vector<Vector>& keys,
                     vector<Vector>& values);
  };
  ```
  
- [ ] 构造函数
  - [ ] 初始化 4 个权重矩阵
  - [ ] 计算 head_dim = n_embd / n_head
  
- [ ] Forward 过程
  - [ ] 投影：q = Wq·x, k = Wk·x, v = Wv·x
  - [ ] 追加到 keys/values cache
  - [ ] 多头并行：
    - [ ] 每个头处理一段维度
    - [ ] 拼接结果
  - [ ] 输出投影：o = Wo·concat
  
- [ ] 测试多头 attention
  - [ ] 2 头、2 token
  - [ ] 对比 Python 输出

#### 📝 测试检查点
```cpp
void test_attention() {
    AttentionBlock attn(n_embd=16, n_head=4);
    Vector x = random_vector(16);
    vector<Vector> keys, values;
    
    Vector out = attn.forward(x, keys, values);
    assert(out.size() == 16);
}
```

**预计时间：2天**  
**里程碑：Attention 机制能工作**

---

### Day 7-8: MLP 和 GPT 组装

#### ☐ 4.1 MLP 模块
- [ ] 实现 `MLPBlock` 类
  ```cpp
  class MLPBlock {
      Matrix fc1, fc2;
      
      Vector forward(const Vector& x);
  };
  ```
  
- [ ] Forward 过程
  - [ ] h = relu(fc1 · x)
  - [ ] out = fc2 · h
  
- [ ] 测试 MLP
  - [ ] 随机输入，检查维度

#### ☐ 4.2 GPT 模型
- [ ] 实现 `GPT` 类
  ```cpp
  class GPT {
      Matrix wte, wpe, lm_head;
      vector<AttentionBlock> attns;
      vector<MLPBlock> mlps;
      int n_layer, n_embd, block_size;
      
      Vector forward(int token_id, int pos_id,
                     vector<vector<Vector>>& keys,
                     vector<vector<Vector>>& values);
  };
  ```
  
- [ ] 构造函数
  - [ ] 初始化 embeddings
  - [ ] 创建 n_layer 个 Attention + MLP
  - [ ] 初始化 lm_head
  
- [ ] Forward 过程
  - [ ] Embedding：x = wte[token] + wpe[pos]
  - [ ] RMSNorm
  - [ ] 对每一层：
    - [ ] x_res = x
    - [ ] x = rmsnorm(x)
    - [ ] x = attention(x) + x_res  // 残差
    - [ ] x_res = x
    - [ ] x = rmsnorm(x)
    - [ ] x = mlp(x) + x_res  // 残差
  - [ ] logits = lm_head · x
  
- [ ] 测试 GPT forward
  - [ ] 单 token，单层
  - [ ] 检查维度：输出 = vocab_size

#### 📝 测试检查点
```cpp
void test_gpt_forward() {
    GPT model(vocab_size=28, n_embd=16, n_layer=1, n_head=4);
    vector<vector<Vector>> keys(1), values(1);
    
    Vector logits = model.forward(0, 0, keys, values);
    assert(logits.size() == 28);
}
```

**预计时间：2天**  
**里程碑：能前向传播得到 logits**

---

## 第三周：训练和推理（优先级★★★★☆）

### Day 9-10: 数据处理

#### ☐ 5.1 Tokenizer
- [ ] 实现 `Tokenizer` 类
  ```cpp
  class Tokenizer {
      vector<char> vocab;
      map<char, int> char_to_id;
      int BOS;
      
      void build_vocab(const vector<string>& docs);
      vector<int> encode(const string& text);
      string decode(const vector<int>& tokens);
  };
  ```
  
- [ ] `build_vocab()`
  - [ ] 收集所有字符
  - [ ] 去重排序
  - [ ] 添加 BOS token
  
- [ ] `encode()` / `decode()`
  - [ ] 字符 ↔ token id 转换
  
- [ ] 测试 tokenizer
  - [ ] "hello" → [?, ?, ?, ?, ?] → "hello"

#### ☐ 5.2 Dataset
- [ ] 实现 `Dataset` 类
  ```cpp
  class Dataset {
      vector<string> docs;
      
      void load(const string& path);
      const string& operator[](size_t idx) const;
      size_t size() const;
  };
  ```
  
- [ ] `load()`
  - [ ] 读取文件
  - [ ] 过滤空行
  - [ ] 随机打乱
  
- [ ] 测试 dataset
  - [ ] 加载 input.txt
  - [ ] 打印前 5 个样本

#### 📝 测试检查点
```cpp
void test_data() {
    Dataset data;
    data.load("input.txt");
    
    Tokenizer tok;
    tok.build_vocab(data.get_all());
    
    auto tokens = tok.encode("emma");
    cout << "Tokens: ";
    for (int t : tokens) cout << t << " ";
}
```

**预计时间：1天**  
**里程碑：能加载并分词数据**

---

### Day 11-12: 训练循环

#### ☐ 6.1 Adam 优化器
- [ ] 实现 `AdamOptimizer` 类
  ```cpp
  class AdamOptimizer {
      vector<double> m, v;
      double lr, beta1, beta2, eps;
      int step_count;
      
      void step(vector<Value*>& params);
      void zero_grad(vector<Value*>& params);
  };
  ```
  
- [ ] 构造函数
  - [ ] 初始化 m, v 为 0
  - [ ] 设置超参数
  
- [ ] `step()` 更新
  - [ ] m = β₁·m + (1-β₁)·grad
  - [ ] v = β₂·v + (1-β₂)·grad²
  - [ ] m̂ = m / (1 - β₁^t)
  - [ ] v̂ = v / (1 - β₂^t)
  - [ ] p -= lr · m̂ / (√v̂ + ε)
  
- [ ] 测试 Adam
  - [ ] 简单函数 f(x) = x²
  - [ ] 验证收敛到 x=0

#### ☐ 6.2 训练主循环
- [ ] 实现 `train()` 函数
  ```cpp
  void train(GPT& model, 
             const Dataset& data,
             const Tokenizer& tok,
             int num_steps);
  ```
  
- [ ] 训练步骤：
  - [ ] 采样一个文档
  - [ ] Tokenize + 添加 BOS
  - [ ] 前向传播：
    - [ ] 逐 token 前向
    - [ ] 计算每个位置的 loss
    - [ ] 累积总 loss
  - [ ] 反向传播：
    - [ ] loss.backward()
  - [ ] 优化器更新
  - [ ] 清零梯度
  - [ ] 打印 loss
  
- [ ] 添加学习率衰减
  - [ ] lr_t = lr * (1 - step / num_steps)
  
- [ ] 测试训练
  - [ ] 运行 10 步
  - [ ] 检查 loss 是否下降

#### 📝 测试检查点
```bash
step    1 / 1000 | loss 3.4567
step    2 / 1000 | loss 3.4123
...
step 1000 / 1000 | loss 1.2345
```

**预计时间：2天**  
**里程碑：能训练模型，loss 下降**

---

### Day 13-14: 推理和测试

#### ☐ 7.1 推理采样
- [ ] 实现 `sample()` 函数
  ```cpp
  int sample(const Vector& probs, 
             double temperature,
             mt19937& rng);
  ```
  
- [ ] 温度缩放
  - [ ] logits / temperature
  
- [ ] 多项式采样
  - [ ] discrete_distribution
  
- [ ] 实现 `generate()` 函数
  ```cpp
  string generate(GPT& model,
                  const Tokenizer& tok,
                  int max_len,
                  double temperature);
  ```
  
- [ ] 生成步骤：
  - [ ] 从 BOS 开始
  - [ ] 逐步前向 + 采样
  - [ ] 遇到 BOS 停止
  - [ ] decode 返回字符串

#### ☐ 7.2 端到端测试
- [ ] 完整流程测试
  - [ ] 加载数据
  - [ ] 训练 100 步
  - [ ] 生成 5 个样本
  
- [ ] 对比 Python 版本
  - [ ] 相同随机种子
  - [ ] 对比 loss 曲线
  - [ ] 对比生成质量
  
- [ ] 数值一致性测试
  - [ ] 单步前向对比
  - [ ] 梯度对比（小模型）

#### 📝 测试检查点
```bash
--- inference ---
sample  1: emma
sample  2: oscar
sample  3: alexander
...
```

**预计时间：2天**  
**里程碑：能生成合理的文本**

---

## 第四周：优化和文档（优先级★★★☆☆）

### Day 15-16: 性能优化（可选）

#### ☐ 8.1 基础优化
- [ ] 编译优化
  - [ ] 使用 `-O3` 标志
  - [ ] 启用 `-march=native`
  
- [ ] 内存优化
  - [ ] 预分配 vector
  - [ ] 使用 `reserve()`
  - [ ] 减少临时对象
  
- [ ] 算法优化
  - [ ] 缓存友好的内存布局
  - [ ] 减少不必要的拷贝
  - [ ] 使用移动语义

#### ☐ 8.2 高级优化（进阶）
- [ ] BLAS 集成
  - [ ] 链接 OpenBLAS/MKL
  - [ ] 用 GEMM 替换矩阵乘法
  
- [ ] 并行化
  - [ ] OpenMP 多头 attention
  - [ ] 并行推理（批量生成）
  
- [ ] SIMD 向量化
  - [ ] 用 Eigen 库
  - [ ] 或手写 AVX 指令

#### 📝 性能测试
```bash
训练 1000 步：
- Python 版本：120 秒
- C++ 基础版：15 秒  (8x)
- C++ 优化版：3 秒   (40x)
```

---

### Day 17-18: 文档和打包

#### ☐ 9.1 代码文档
- [ ] 添加注释
  - [ ] 每个类的作用
  - [ ] 每个函数的参数/返回值
  - [ ] 关键算法的说明
  
- [ ] Doxygen 风格
  ```cpp
  /**
   * @brief 前向传播
   * @param x 输入向量
   * @return 输出 logits
   */
  Vector forward(const Vector& x);
  ```

#### ☐ 9.2 README 和示例
- [ ] 写 README.md
  - [ ] 项目介绍
  - [ ] 编译说明
  - [ ] 使用示例
  - [ ] 对比 Python 版本
  
- [ ] 提供示例代码
  - [ ] 训练脚本
  - [ ] 推理脚本
  - [ ] 自定义数据集

#### ☐ 9.3 CMake 构建
- [ ] 编写 CMakeLists.txt
  ```cmake
  cmake_minimum_required(VERSION 3.15)
  project(MicroGPT)
  
  add_executable(train src/train.cpp ...)
  add_executable(inference src/inference.cpp ...)
  add_executable(tests tests/test_*.cpp ...)
  ```
  
- [ ] 支持不同编译模式
  - [ ] Debug：带调试信息
  - [ ] Release：优化性能
  
- [ ] 依赖管理
  - [ ] 可选：OpenBLAS
  - [ ] 可选：Eigen

---

## 验收标准

### ✅ 功能完整性
- [ ] 能加载数据并分词
- [ ] 能训练模型（loss 下降）
- [ ] 能生成文本（不是乱码）
- [ ] 结果与 Python 版本接近

### ✅ 代码质量
- [ ] 无内存泄漏（valgrind 检查）
- [ ] 单元测试通过
- [ ] 代码有注释
- [ ] 遵循 C++ 规范

### ✅ 性能
- [ ] 训练速度 > Python 版本 5x
- [ ] 推理速度 > Python 版本 10x
- [ ] 内存占用合理

### ✅ 文档
- [ ] README 清晰
- [ ] 有编译和运行说明
- [ ] 有测试结果对比

---

## 常见问题排查

### 问题：编译错误
```bash
# 检查 C++ 标准
g++ --version  # 需要 >= 11

# 检查 CMake 版本
cmake --version  # 需要 >= 3.15
```

### 问题：运行时崩溃
```bash
# 使用 valgrind 检测内存问题
valgrind --leak-check=full ./train

# 使用 gdb 调试
gdb ./train
(gdb) run
(gdb) backtrace
```

### 问题：loss 不下降
- [ ] 检查学习率（试试 0.001, 0.01, 0.1）
- [ ] 检查梯度是否为 0
- [ ] 检查是否忘记 backward()
- [ ] 检查是否忘记清零梯度

### 问题：生成乱码
- [ ] 训练步数太少（至少 500 步）
- [ ] 温度设置不对（试试 0.8）
- [ ] 模型容量太小（增加 n_embd 或 n_layer）

---

## 开发建议

### 🎯 优先级排序
1. **必做**（第一周）：Value, Matrix, 基础算子
2. **必做**（第二周）：Attention, MLP, GPT
3. **必做**（第三周）：训练循环，推理采样
4. **可选**（第四周）：性能优化

### 🔄 迭代开发
- 先实现最简单的版本（1层，4维）
- 测试通过后再扩展
- 不要一次性写太多代码

### 🧪 持续测试
- 每完成一个模块就写测试
- 用 Python 版本验证正确性
- 出错立即定位，不要累积

### 📊 可视化调试
- 打印中间值（loss, grad, attention weights）
- 画 loss 曲线（Matplotlib）
- 比较 Python/C++ 的差异

---

## 参考资源

### 学习资料
- [MicroGPT Python 版本](microgpt.py) - 原始实现
- [GPT 工作流程详解](GPT_WORKFLOW_CN.md) - 原理说明
- [C++ 实现规划](CPP_IMPLEMENTATION_PLAN.md) - 详细计划

### C++ 库
- [Eigen](https://eigen.tuxfamily.org/) - 矩阵运算
- [OpenBLAS](https://www.openblas.net/) - 线性代数加速
- [Google Test](https://github.com/google/googletest) - 单元测试

### 工具
- [Valgrind](https://valgrind.org/) - 内存检测
- [GDB](https://www.gnu.org/software/gdb/) - 调试器
- [CMake](https://cmake.org/) - 构建系统

---

## 完成后的收获

✨ **技术能力**
- 深入理解 Transformer 架构
- 掌握自动求导原理
- 熟练使用现代 C++

💡 **工程能力**
- 大型项目的架构设计
- 测试驱动开发
- 性能优化技巧

🚀 **可扩展性**
- 可以继续改进（批处理、量化、分布式）
- 可以应用到其他模型（BERT, ViT）
- 可以作为简历项目

加油！一步一步来，你一定可以完成的！💪
