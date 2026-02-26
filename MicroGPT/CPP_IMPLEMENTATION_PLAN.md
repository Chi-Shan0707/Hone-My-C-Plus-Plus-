# MicroGPT C++ 实现规划

## 一、GPT 工作流程分析

### 1.1 GPT 基本原理
GPT（Generative Pre-trained Transformer）是一个**自回归语言模型**：
- **目标**：根据前面的token序列，预测下一个token
- **训练方式**：给定序列 [t₀, t₁, ..., tₙ]，在每个位置i预测 t_{i+1}
- **核心思想**：学习token之间的条件概率分布 P(t_{i+1} | t₀, t₁, ..., tᵢ)

### 1.2 Transformer 架构关键组件
1. **Embedding层**：
   - Token Embedding：将离散token映射到连续向量空间
   - Position Embedding：注入位置信息（Transformer本身无位置概念）

2. **Multi-Head Self-Attention**：
   - Query (Q)、Key (K)、Value (V) 三个投影
   - Attention Score = softmax(Q·K^T / √d) 
   - Output = AttentionScore · V
   - 多头机制：从不同子空间并行学习关系模式

3. **Feed-Forward MLP**：
   - 简单的两层全连接网络（扩展→ReLU→压缩）
   - 负责特征变换和信息重编码

4. **残差连接 + 归一化**：
   - 残差：x_out = f(x) + x，保留原始信息流
   - RMSNorm：只按均方根缩放，计算比LayerNorm更简单

---

## 二、microgpt.py 运行流程详解

### 2.1 数据准备阶段
```
输入：input.txt（每行一个样本，例如人名）
↓
字符级tokenizer：统计所有出现的字符 → 构建词表
↓
添加特殊token：BOS（序列边界标记）
↓
样本编码：每个字符串转为 token id 序列
```

### 2.2 模型初始化
```
参数结构：
├── wte (vocab_size × n_embd)          # token embedding
├── wpe (block_size × n_embd)          # position embedding
├── layer0...layerN (每层包含)
│   ├── attn_wq/wk/wv/wo (n_embd × n_embd)  # attention权重
│   └── mlp_fc1/fc2                          # MLP权重
└── lm_head (vocab_size × n_embd)      # 输出投影

初始化方式：高斯随机 N(0, 0.08)
```

### 2.3 训练循环（核心流程）
```python
for step in range(num_steps):
    # 1) 数据准备
    doc = docs[step % len(docs)]
    tokens = [BOS] + [字符→token_id] + [BOS]
    
    # 2) 前向传播
    keys, values = []  # 每层的K/V缓存
    for pos_id in range(序列长度):
        token_id = tokens[pos_id]
        target_id = tokens[pos_id + 1]  # 下一个token作为标签
        
        # 2.1) Embedding
        x = tok_emb[token_id] + pos_emb[pos_id]
        
        # 2.2) Transformer层
        for layer in layers:
            # Attention Block
            x = rmsnorm(x)
            q, k, v = 投影(x)
            keys[layer].append(k)  # 累积历史
            values[layer].append(v)
            
            # 多头attention
            for head in heads:
                scores = q_head · k_history / √d
                weights = softmax(scores)
                head_out = weights · v_history
            x = attention_output + x  # 残差
            
            # MLP Block
            x = rmsnorm(x)
            x = fc2(relu(fc1(x)))
            x = mlp_output + x  # 残差
        
        # 2.3) 输出logits
        logits = lm_head(x)
        probs = softmax(logits)
        loss_t = -log(probs[target_id])  # 交叉熵损失
    
    # 3) 反向传播
    loss = mean(loss_t for all positions)
    loss.backward()  # 自动求导计算梯度
    
    # 4) 优化器更新
    for param in params:
        # Adam：动量 + 自适应学习率
        m = β₁·m + (1-β₁)·grad
        v = β₂·v + (1-β₂)·grad²
        param -= lr · m̂ / (√v̂ + ε)
        param.grad = 0

    print(f"step {step} | loss {loss}")
```

### 2.4 推理采样
```python
# 从BOS开始自回归生成
token_id = BOS
sample = []
keys, values = []

for pos in range(max_length):
    logits = gpt(token_id, pos, keys, values)
    probs = softmax(logits / temperature)  # 温度控制多样性
    token_id = random.choices(tokens, weights=probs)  # 概率采样
    
    if token_id == BOS:
        break  # 遇到结束符停止
    sample.append(token_id)

return decode(sample)  # token转回字符串
```

---

## 三、C++ 实现步骤规划

### 阶段1：基础设施层 (1-2天)
**优先级：最高 | 依赖：无**

#### 1.1 自动求导系统 `Value` 类
```cpp
// 目标：支持标量的自动微分
class Value {
    double data;                      // 前向数值
    double grad;                      // 反向梯度
    vector<Value*> children;          // 计算图依赖
    vector<double> local_grads;       // 局部导数
    
    // 运算符重载：+, -, *, /, pow, exp, log, relu
    Value operator+(const Value& other);
    Value operator*(const Value& other);
    // ...
    
    void backward();  // 拓扑反传
};
```

**C++优势**：
- 可用RAII管理计算图内存
- 运算符重载更自然（不需要`__add__`这种特殊方法）
- 编译期类型检查避免运行时错误

#### 1.2 矩阵容器 `Matrix` 类
```cpp
// 目标：2D矩阵 + 基础运算
class Matrix {
    vector<vector<Value>> data;
    size_t rows, cols;
    
    Matrix(size_t r, size_t c, double mean=0, double std=0.08);
    Vector operator*(const Vector& vec);  // 矩阵-向量乘
    Value& at(size_t i, size_t j);
};

using Vector = vector<Value>;  // 1D向量
```

**为什么不直接用Eigen/Armadillo？**
- 这些库不支持自动求导，我们的`Value`类需要追踪计算图
- 后续优化阶段可以考虑将标量Value升级为张量Value，再结合BLAS加速

---

### 阶段2：神经网络算子 (2-3天)
**优先级：高 | 依赖：Value, Matrix**

#### 2.1 实现基础函数
```cpp
// linear.hpp
Vector linear(const Vector& x, const Matrix& W);

// activation.hpp
Vector relu(const Vector& x);
Vector softmax(const Vector& logits);  // 数值稳定版
Vector rmsnorm(const Vector& x);
```

**C++优势**：
- 预分配内存避免频繁allocation（Python每次列表推导都会分配）
- 循环展开、SIMD指令优化潜力（后期）
- 栈上小向量性能更好（std::array）

#### 2.2 测试各算子正确性
```cpp
// test_operators.cpp
void test_softmax() {
    Vector x = {1.0, 2.0, 3.0};
    Vector probs = softmax(x);
    assert(abs(sum(probs) - 1.0) < 1e-6);  // 概率和为1
}
```

---

### 阶段3：GPT 前向传播 (3-4天)
**优先级：高 | 依赖：算子层**

#### 3.1 Attention 模块
```cpp
// attention.hpp
struct AttentionBlock {
    Matrix wq, wk, wv, wo;
    int n_embd, n_head, head_dim;
    
    Vector forward(const Vector& x, 
                   vector<Vector>& keys,    // K缓存
                   vector<Vector>& values); // V缓存
};
```

**实现细节**：
- 多头attention：每个头处理子空间 [h*head_dim : (h+1)*head_dim]
- K/V cache：自回归推理时避免重复计算历史token
- Attention score除以√d：控制softmax梯度尺度

#### 3.2 MLP 模块
```cpp
// mlp.hpp
struct MLPBlock {
    Matrix fc1, fc2;  // fc1: n_embd→4*n_embd, fc2: 4*n_embd→n_embd
    
    Vector forward(const Vector& x) {
        Vector h = relu(linear(x, fc1));
        return linear(h, fc2);
    }
};
```

#### 3.3 完整GPT前向
```cpp
// gpt.hpp
struct GPT {
    Matrix wte, wpe, lm_head;
    vector<AttentionBlock> attns;
    vector<MLPBlock> mlps;
    
    Vector forward(int token_id, int pos_id,
                   vector<vector<Vector>>& keys,
                   vector<vector<Vector>>& values);
};
```

**C++优势**：
- 结构化设计：类封装清晰（Python只用函数和字典）
- 显式内存布局：可优化缓存友好性
- 可选的并行化：OpenMP并行多头attention

---

### 阶段4：训练循环 (3-4天)
**优先级：高 | 依赖：GPT前向**

#### 4.1 Adam 优化器
```cpp
// optimizer.hpp
class AdamOptimizer {
    vector<double> m, v;  // 动量缓冲
    double lr, beta1, beta2, eps;
    int step = 0;
    
    void step(vector<Value*>& params) {
        step++;
        for (int i = 0; i < params.size(); i++) {
            m[i] = beta1*m[i] + (1-beta1)*params[i]->grad;
            v[i] = beta2*v[i] + (1-beta2)*params[i]->grad*params[i]->grad;
            
            double m_hat = m[i] / (1 - pow(beta1, step));
            double v_hat = v[i] / (1 - pow(beta2, step));
            
            params[i]->data -= lr * m_hat / (sqrt(v_hat) + eps);
            params[i]->grad = 0;
        }
    }
};
```

#### 4.2 训练主循环
```cpp
// train.cpp
void train(GPT& model, const Dataset& data, int num_steps) {
    AdamOptimizer opt(params, lr=0.01);
    
    for (int step = 0; step < num_steps; step++) {
        // 1) 采样 + tokenize
        auto doc = data[step % data.size()];
        vector<int> tokens = {BOS} + tokenize(doc) + {BOS};
        
        // 2) 前向
        vector<vector<Vector>> keys(n_layer), values(n_layer);
        Value loss = 0;
        for (int pos = 0; pos < tokens.size()-1; pos++) {
            Vector logits = model.forward(tokens[pos], pos, keys, values);
            Vector probs = softmax(logits);
            loss += -probs[tokens[pos+1]].log();
        }
        loss = loss / (tokens.size()-1);
        
        // 3) 反向 + 更新
        loss.backward();
        opt.step();
        
        if (step % 100 == 0)
            cout << "step " << step << " | loss " << loss.data << endl;
    }
}
```

**C++优势**：
- 内存复用：keys/values缓存可在循环外预分配
- 早停逻辑：可方便添加验证集监控
- Checkpoint保存：二进制序列化比Python pickle快

---

### 阶段5：推理采样 (1天)
**优先级：中 | 依赖：训练完成**

#### 5.1 采样策略
```cpp
// sampling.hpp
int sample(const Vector& probs, double temperature, mt19937& rng) {
    // 温度缩放 + 多项式采样
    vector<double> scaled_probs;
    for (auto& p : probs)
        scaled_probs.push_back(p.data);
    
    discrete_distribution<> dist(scaled_probs.begin(), scaled_probs.end());
    return dist(rng);
}
```

#### 5.2 生成函数
```cpp
// inference.cpp
string generate(GPT& model, int max_len=16, double temp=0.5) {
    vector<vector<Vector>> keys(n_layer), values(n_layer);
    int token_id = BOS;
    vector<int> generated;
    
    for (int pos = 0; pos < max_len; pos++) {
        Vector logits = model.forward(token_id, pos, keys, values);
        Vector probs = softmax(logits / temp);
        token_id = sample(probs, temp, rng);
        
        if (token_id == BOS) break;
        generated.push_back(token_id);
    }
    
    return decode(generated);
}
```

**C++优势**：
- std::mt19937：质量更高的随机数生成器
- 可选的Beam Search：C++更容易实现高效的优先队列
- 批量推理：后续可同时生成多个样本

---

### 阶段6：数据处理 (1-2天)
**优先级：中 | 依赖：无（可并行开发）**

#### 6.1 Tokenizer
```cpp
// tokenizer.hpp
class Tokenizer {
    vector<char> vocab;      // 字符词表
    map<char, int> char_to_id;
    int BOS;
    
    vector<int> encode(const string& text);
    string decode(const vector<int>& tokens);
};
```

**C++优势**：
- std::map查找：O(log V)（Python dict虽然O(1)但常数大）
- UTF-8支持：可用ICU库或codecvt处理多语言
- 内存映射：大文件用mmap而非一次性读入

#### 6.2 数据加载
```cpp
// dataset.hpp
class Dataset {
    vector<string> docs;
    
    void load(const string& path) {
        ifstream file(path);
        string line;
        while (getline(file, line))
            if (!line.empty()) docs.push_back(line);
        
        shuffle(docs.begin(), docs.end(), rng);
    }
};
```

---

### 阶段7：测试与对齐 (2-3天)
**优先级：高 | 依赖：所有模块**

#### 7.1 单元测试
```cpp
// 测试每个组件与Python版本的数值一致性
void test_softmax_parity();
void test_attention_parity();
void test_forward_parity();
```

工具：
- 从Python导出中间结果（numpy.save）
- C++读取并对比（允许1e-5浮点误差）

#### 7.2 端到端验证
```bash
# Python版本
python microgpt.py > py_output.txt

# C++版本
./microgpt > cpp_output.txt

# 对比损失曲线
diff <(grep "loss" py_output.txt) <(grep "loss" cpp_output.txt)
```

---

### 阶段8：性能优化（可选，3-5天）
**优先级：低 | 依赖：功能完整**

#### 8.1 计算优化
- **矩阵乘法**：调用OpenBLAS/MKL的GEMM
- **SIMD向量化**：用Eigen或手写AVX2指令
- **并行化**：
  - 多头attention并行（OpenMP）
  - 批处理推理（同时生成多个样本）

#### 8.2 内存优化
- **对象池**：预分配Value/Vector对象
- **梯度累积**：大batch分多次前向再一次反向
- **混合精度**：float16前向 + float32梯度

#### 8.3 量化（高级）
- **推理量化**：权重int8，激活int8/int16
- **QAT训练**：训练时模拟量化误差

**预期加速比**：
- 基础优化（BLAS + 内存池）：5-10x
- 完整优化（SIMD + 并行）：20-50x
- 量化部署：50-100x

---

## 四、C++ 相比 Python 的优势总结

### 4.1 性能优势
| 方面 | Python | C++ | 提升 |
|------|--------|-----|------|
| 数值计算 | 解释器开销 | 编译原生码 | 10-50x |
| 内存分配 | GC频繁触发 | 手动/RAII | 2-5x |
| 循环 | 慢 | 展开+SIMD | 5-20x |
| 多线程 | GIL限制 | 真并行 | 接近线性 |

### 4.2 工程优势
1. **类型安全**：编译期捕获维度错误（`Matrix<16,4> * Vector<3>` 会报错）
2. **接口清晰**：类的public/private明确对外接口
3. **内存可控**：避免Python的内存碎片和泄漏
4. **部署简单**：单个可执行文件，无需Python环境

### 4.3 学习优势
1. **理解更深**：手动管理内存→理解计算图生命周期
2. **优化空间大**：可尝试缓存优化、指令流水线等底层技术
3. **生产就绪**：C++实现更接近工业级框架（PyTorch C++后端）

---

## 五、开发时间表（总计10-15天）

| 阶段 | 天数 | 关键产出 | 可验证点 |
|------|------|----------|----------|
| 1. 基础设施 | 2 | Value类、Matrix类 | test_value.cpp通过 |
| 2. 神经网络算子 | 3 | linear、softmax、rmsnorm | test_operators.cpp |
| 3. GPT前向 | 4 | Attention、MLP、GPT | forward_parity_test |
| 4. 训练循环 | 4 | Adam优化器、训练主循环 | loss曲线下降 |
| 5. 推理采样 | 1 | 生成函数 | 能生成合理样本 |
| 6. 数据处理 | 2 | Tokenizer、Dataset | 加载input.txt |
| 7. 测试对齐 | 3 | 单元测试、端到端测试 | 与Python结果一致 |
| 8. 性能优化（可选） | 5 | BLAS、SIMD、并行 | 速度提升10x+ |

---

## 六、建议的实现顺序

### 第一周：核心功能
1. **Day 1-2**：Value类（最核心，后续都依赖它）
2. **Day 3-4**：Matrix类 + 基础算子（linear、relu、softmax）
3. **Day 5-6**：Attention块（最复杂的模块）
4. **Day 7**：MLP块 + GPT组装

### 第二周：训练与测试
1. **Day 8-9**：Tokenizer + Dataset
2. **Day 10-11**：训练循环 + Adam优化器
3. **Day 12**：推理采样
4. **Day 13-14**：全面测试，对齐Python版本
5. **Day 15**：文档与示例

---

## 七、调试建议

### 7.1 渐进式验证
```
Value类 → 手算3个节点的图 → 比对梯度
Matrix乘法 → 2x2矩阵×2维向量 → 手算验证
Softmax → 简单输入[1,2,3] → 验证和为1
Attention → 单头、2个token → 打印中间值
GPT → 单层、单token → 逐层比对Python
```

### 7.2 常见坑
1. **梯度不累积**：`child.grad += ...` 而非 `=`
2. **残差连接忘加**：导致梯度消失
3. **除以√d忘记**：attention分数爆炸
4. **K/V cache重置**：每个样本要清空缓存
5. **学习率太大**：Adam建议0.001-0.01

---

## 八、参考资料

### 8.1 原理学习
- [The Illustrated Transformer](http://jalammar.github.io/illustrated-transformer/)
- [Karpathy的nanoGPT](https://github.com/karpathy/nanoGPT)（PyTorch版本）
- [Attention is All You Need](https://arxiv.org/abs/1706.03762)（原始论文）

### 8.2 C++工程
- Eigen库文档：矩阵运算参考
- Google Test：单元测试框架
- CMake教程：现代C++构建系统

---

## 九、下一步行动

### 立即可开始
1. 创建项目结构：
```
MicroGPT/
├── src/
│   ├── value.hpp/cpp       # 自动求导
│   ├── matrix.hpp/cpp      # 矩阵容器
│   ├── operators.hpp/cpp   # 神经网络算子
│   ├── attention.hpp/cpp   # Attention模块
│   ├── mlp.hpp/cpp         # MLP模块
│   ├── gpt.hpp/cpp         # GPT模型
│   ├── optimizer.hpp/cpp   # Adam优化器
│   ├── tokenizer.hpp/cpp   # 分词器
│   └── main.cpp            # 训练+推理主程序
├── tests/
│   └── test_*.cpp          # 单元测试
├── CMakeLists.txt
└── README.md
```

2. 从`value.hpp`开始，实现第一个能运行的自动求导例子：
```cpp
Value a(2.0), b(3.0);
Value c = a * b + a;  // c = 2*3 + 2 = 8
c.backward();
cout << a.grad << endl;  // 应输出 4.0 (dc/da = b + 1 = 4)
```

3. 每完成一个模块就写对应测试，确保正确性后再继续。

---

**总结**：C++实现microGPT不仅能大幅提升性能，更重要的是能深入理解每个细节（内存布局、计算图、优化器状态等）。建议先快速搭建一个能跑通的版本（阶段1-7），再考虑性能优化（阶段8）。祝开发顺利！

