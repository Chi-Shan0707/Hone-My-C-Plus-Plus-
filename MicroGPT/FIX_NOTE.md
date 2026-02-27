# Value 生命周期修复方案

## 问题诊断
当前 loss 不下降原因：
1. ✅ 中间计算的 Value 已放入 `global_value_pool` (list)，指针有效
2. ❌ **权重参数的 Value 还在 `Vector/Matrix.data` 中**，是 `vector<Value>` 而非指针
3. ❌ backward() 时，children 指向的权重 Value 的 grad **没有被正确传回**

## 根本原因
- `Matrix.data` 中的 Value 因为被复制，grad 信息传递时会丢失
- 不能简单地用 list 解决，因为权重不在池中

## 解决方案（需要更大改动）
需要将整个系统改为使用 `shared_ptr<Value>`：
1. `Vector.data`: `vector<Value>` → `vector<shared_ptr<Value>>`
2. `Matrix.data`: `vector<vector<Value>>` → `vector<vector<shared_ptr<Value>>>>`
3. `Value.children`: `vector<Value*>` → `vector<shared_ptr<Value>>`
4. 所有 operator 返回 `shared_ptr<Value>`

## 当前状态
使用了全局 `list<Value> global_value_pool` 让中间值不被销毁，但这个方案**不彻底**。

等待后续使用 shared_ptr 的完整重构。
