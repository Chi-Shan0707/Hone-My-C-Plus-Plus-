#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <list>
#include <cmath>
#include <map>
#include <random>
#include <limits>
using namespace std;

// ── 全局超参数（训练前在这里统一调整）────────────────────
struct Config {
    int vocab_size = 27;
    int n_embd     = 16;
    int n_head     = 4;
    int n_layer    = 2;
    int n_hidden   = 64;
    int block_size = 32;
} cfg;

// ── 全局值池（"笨拙"方案）：list 保证 push_back 不使指针失效──────
list<struct Value> global_value_pool;


class Value
{
public:
    double data; //前向数值
    double grad; //反向梯度
    vector<Value*> children; //计算图中的子节点
    vector<double> local_grads; //局部导数

    Value() : data(0.0), grad(0.0) {}   //构造函数
    explicit Value(double d) : data(d), grad(0.0) {}//显示构造函数


    Value operator+(const Value& other) const {
    // c = a + b，计算结果在 global_value_pool 中分配，永不销毁
        global_value_pool.emplace_back();
        Value* result = &global_value_pool.back();
        result->data = this->data + other.data;
        result->children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result->local_grads = {1.0, 1.0};
        return *result;
    }
    Value operator*(const Value& other) const {
    // c = a * b
        global_value_pool.emplace_back();
        Value* result = &global_value_pool.back();
        result->data = this->data * other.data;
        result->children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result->local_grads = {other.data, this->data};
        return *result;
    }
    Value operator-(const Value& other) const {
    // c = a - b
        global_value_pool.emplace_back();
        Value* result = &global_value_pool.back();
        result->data = this->data - other.data;
        result->children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result->local_grads = {1.0, -1.0};
        return *result;
    }
    Value operator/(const Value& other) const {
    // c = a / b
        global_value_pool.emplace_back();
        Value* result = &global_value_pool.back();
        result->data = this->data / other.data;
        result->children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result->local_grads = {1.0 / other.data, -this->data / (other.data * other.data)};
        return *result;
    }
    Value log() const {
    // c = log(a)，导数 dc/da = 1/a
        global_value_pool.emplace_back();
        Value* result = &global_value_pool.back();
        result->data = std::log(this->data);
        result->children = {const_cast<Value*>(this)};
        result->local_grads = {1.0 / this->data};
        return *result;
    }
    // Value& operator+=(const Value& other) {
    //     *this = *this + other; // 复用已有的 operator+
    //     return *this;
    // }

    void backward() {
        // 反向传播算法
        // 1. 拓扑排序
        vector<Value*> topo;
        set<Value*> visited;
        build_topo(this, topo, visited);
        
        // 2. 初始化：损失对自身的梯度为 1
        this->grad = 1.0;
        
        // 3. 逆序传播梯度
        for (auto it = topo.rbegin(); it != topo.rend(); ++it)
        {
            Value* v = *it;
            for (size_t i = 0; i < v->children.size(); ++i) 
            {
                Value* child = v->children[i];
                double local_grad = v->local_grads[i];
                child->grad += v->grad * local_grad; // 累积梯度
            }
        }
    }
private :
    void build_topo(Value* v, vector<Value*>& topo, set<Value*>& visited)
    //万事皆可取引用
    {
        if (visited.find(v) != visited.end())
        {
            return;  // 已访问过
        }
        
        visited.insert(v);
        
        // 递归访问所有子节点
        for (auto& child : v->children) {
            build_topo(child, topo, visited);
        }
        
        // DFS 后序：先访问子节点，再加入自己
        topo.push_back(v);
    }
};

class Vector
{
public:
    vector<Value> data;
};

class Matrix
{

public:

    vector<vector<Value>> data;
    size_t row;
    size_t col;

    Vector operator*(const Vector& vec) const {
        Vector result;
        result.data.resize(row);
        for (size_t i = 0; i < row; ++i) {
            Value sum(0.0);
            for (size_t j = 0; j < col; ++j) {
                sum = sum + data[i][j] * vec.data[j];
            }
            result.data[i] = sum;
        }
        return result;
    }

};

Vector linear(const Matrix& weights, const Vector& input)
{
    return weights * input;
}


Vector softmax(const Vector& logits)
{
    Vector result;
    double max_val = -std::numeric_limits<double>::infinity();

    for (const auto& val : logits.data) {
        max_val = (max_val > val.data) ? max_val : val.data; //数值稳定性：减去最大值
    }
    
    double sum_exp = 0.0;
    for (const auto& val : logits.data) {
        sum_exp += exp(val.data - max_val);
    }
    
    result.data.resize(logits.data.size());
    for (size_t i = 0; i < logits.data.size(); ++i) {
        result.data[i] = Value(exp(logits.data[i].data - max_val) / sum_exp);
    }
    
    return result;
}


Vector relu(const Vector& input)
{
    Vector result;
    result.data.resize(input.data.size());
    for (size_t i = 0; i < input.data.size(); ++i)
        result.data[i] = (input.data[i].data > 0) ? input.data[i] : Value(0.0);
    return result;
}

// 向量点积：a·b = Σ a[i]*b[i]
double dot(const Vector& a, const Vector& b)
{
    double sum = 0.0;
    for (size_t i = 0; i < a.data.size(); ++i)
        sum += a.data[i].data * b.data[i].data;
    return sum;
}

// 标量乘向量：s * v
Vector scale(double s, const Vector& v)
{
    Vector result;
    result.data.resize(v.data.size());
    for (size_t i = 0; i < v.data.size(); ++i)
        result.data[i] = Value(s * v.data[i].data);
    return result;
}

// 向量加法：a + b（逐元素）
Vector add(const Vector& a, const Vector& b)
{
    Vector result;
    result.data.resize(a.data.size());
    for (size_t i = 0; i < a.data.size(); ++i)
        result.data[i] = Value(a.data[i].data + b.data[i].data);
    return result;
}

Vector resnorm(const Vector& input)
{
    Vector result;
    double sum_squares = 0.0;
    for (const auto& val : input.data)
        sum_squares += val.data * val.data;
    
    double norm = sqrt(sum_squares);
    result.data.resize(input.data.size());
    for (size_t i = 0; i < input.data.size(); ++i)
        result.data[i] = Value(input.data[i].data / norm);
    
    return result;
}



struct AttentionBlock
{
    Matrix wq, wk, wv; // 权重矩阵
    size_t n_embd, n_head, head_dim;
    Vector forward(const Vector& x, vector<Vector>& keys, vector<Vector>& values)
    {
        // ── 第一步：投影 Q / K / V ──────────────────────────────
        Vector q = wq * x;  // 当前 token："要查什么"
        Vector k = wk * x;  // 当前 token："能提供什么"
        Vector v = wv * x;  // 当前 token："实际内容"

        // 把本次的 k, v 追加到历史缓存（KV cache）
        keys.push_back(k);
        values.push_back(v);

        // ── 第二步：计算每个历史位置的 attention score ─────────
        // score[i] = dot(q, keys[i]) / sqrt(head_dim)
        size_t seq_len = keys.size();
        Vector scores;
        scores.data.resize(seq_len);
        double scale_factor = 1.0 / sqrt((double)head_dim);
        for (size_t i = 0; i < seq_len; ++i)
            scores.data[i] = Value(dot(q, keys[i]) * scale_factor);

        // ── 第三步：softmax → 权重（和为 1）───────────────────
        Vector weights = softmax(scores);

        // ── 第四步：加权求和 values ────────────────────────────
        // output = Σ weights[i] * values[i]
        Vector output;
        output.data.resize(n_embd, Value(0.0));
        for (size_t i = 0; i < seq_len; ++i)
            output = add(output, scale(weights.data[i].data, values[i]));

        return output;
    }
};
struct MLPBlock
//这个写起来比AttentionBlock简单，因为它只有两次线性变换
{

    Matrix w1, w2; // 权重矩阵
    size_t n_embd, n_hidden;
    Vector forward(const Vector& x)
    {
        Vector hidden = relu(linear(w1, x)); 
        /**
         * x = relu(linear(w1, x))  
          
         * 这样会报错，因为我们和编译器说了x不被更改
         * 这也体现了一个好习惯：函数参数用 const& 表达"我只读这个输入"，内部计算结果用局部变量承接。
         * 
         */

        return linear(w2, hidden);           // hidden (n_hidden) → output (n_embd)
    }
};
class Tokenize
{
public:
    vector<char> vocab;        // 词表：下标即 token_id，值为对应字符
    map<char, int> char_to_id; // 字符 → token_id 的映射
    int BOS;                   // Begin-Of-Sequence 特殊 token 的 id

    vector<int> encode(const string& text)
    {
        vector<int> tokens;
        for (char ch : text) {
            auto it = char_to_id.find(ch);
            if (it != char_to_id.end()) {
                tokens.push_back(it->second); // 转换为 token_id
            } else {
                tokens.push_back(BOS); // 未知字符用 BOS 代替
            }
        }
        return tokens;
    }

    string decode(const vector<int>& tokens)
    {
        string text;
        for (int token_id : tokens) {
            if (token_id >= 0 && token_id < (int)vocab.size()) {
                text += vocab[token_id]; // token_id → 字符
            } else {
                text += '?'; // 无效 id 用 '?' 代替
            }
        }
        return text;
    }
};
struct GPT
{
    // ── 嵌入层参数 ────────────────────────────────────────────────────
    Matrix wte;      // Token  Embedding：词表大小 × n_embd
                     //   作用：把 token_id（整数）查表变成 n_embd 维向量
                     //   例：token_id=5 → wte 第5行 → 一个 n_embd 维向量

    Matrix wpe;      // Position Embedding：最大序列长度 × n_embd
                     //   作用：把位置编号（0,1,2,...）变成 n_embd 维向量
                     //   例：pos_id=3 → wpe 第3行 → 一个 n_embd 维向量

    Matrix lm_head;  // 语言模型输出头：vocab_size × n_embd
                     //   作用：把最终隐藏状态映射回词表大小的 logits
                     //   例：n_embd 维向量 → vocab_size 维 logits → softmax → 概率分布

    // ── Transformer 层（每层配对一个 Attention + 一个 MLP）────────────
    vector<AttentionBlock> attn_blocks; // N 层 Attention，每层调用 AttentionBlock::forward
                                        //   输入：当前隐藏状态 x + keys/values 缓存
                                        //   输出：融合了历史信息的新 x（残差连接应在此处）
    vector<MLPBlock>       mlp_blocks;  // N 层 MLP，与 attn_blocks 一一对应
                                        //   输入：Attention 已更新的 x
                                        //   输出：经过非线性变换的新 x（残差连接应在此处）

    // ── 前向传播 ──────────────────────────────────────────────────────
    // 参数说明：
    //   token_id : 当前位置的 token 编号（整数索引）
    //   pos_id   : 当前位置的位置编号（0, 1, 2, ...）
    //   keys     : KV cache —— 所有历史位置的 key 向量，Attention 用来查询历史
    //   values   : KV cache —— 所有历史位置的 value 向量，Attention 用来加权求和
    //
    // 工作流程：
    //   步骤1  Embedding：x = wte[token_id] + wpe[pos_id]
    //   步骤2  逐层串行执行（共 N 层）：
    //            x = x + AttentionBlock.forward(x, keys, values)  ← 残差
    //            x = x + MLPBlock.forward(x)                      ← 残差
    //   步骤3  输出头：logits = lm_head * x   (vocab_size 维)
    //
    // 返回值：logits，之后接 softmax 得到下一个 token 的概率分布
    Vector forward(int token_id, int pos_id, vector<Vector>& keys, vector<Vector>& values)
    {
        // 步骤1：Embedding 层——直接取矩阵对应行（查表，不是矩阵乘法）
        // wte.data[token_id] 就是词表第 token_id 行，即该 token 的 n_embd 维向量
        Vector tok_emb; tok_emb.data = wte.data[token_id]; // Token Embedding
        Vector pos_emb; pos_emb.data = wpe.data[pos_id];   // Position Embedding
        Vector x = add(tok_emb, pos_emb);                  // x = tok_emb + pos_emb

        // 步骤2：逐层 Attention → MLP（串行！不是并行！）
        for (size_t i = 0; i < attn_blocks.size(); ++i) 
        {
            x = attn_blocks[i].forward(x, keys, values); // Attention：聚合历史信息
            x = mlp_blocks[i].forward(x);                // MLP：对聚合结果做非线性变换
            // 注：完整实现还需在此处加 RMSNorm + 残差连接
        }

        // 步骤3：输出头，得到 vocab_size 维 logits
        return lm_head * x;
    }

    // 收集模型所有可训练参数的指针，供优化器使用
    vector<Value*> params()
    {
        vector<Value*> ps;
        // 辅助 lambda：把一个 Matrix 的所有 Value* 追加进 ps
        auto add_matrix = [&](Matrix& m) {
            for (auto& row : m.data)
                for (auto& val : row)
                    ps.push_back(&val);
        };
        add_matrix(wte);
        add_matrix(wpe);
        add_matrix(lm_head);
        for (auto& blk : attn_blocks) {
            add_matrix(blk.wq);
            add_matrix(blk.wk);
            add_matrix(blk.wv);
        }
        for (auto& blk : mlp_blocks) {
            add_matrix(blk.w1);
            add_matrix(blk.w2);
        }
        return ps;
    }
};
void SGD_step( vector<Value*>& params, double lr)
{
    for (auto& param : params) {
        param->data -= lr * param->grad; // 更新参数：θ = θ - lr * dL/dθ
        param->grad = 0.0; // 清零梯度，为下一轮计算做准备
    }
}


void train(GPT& model, const vector<string>& data, Tokenize& tokenizer,
           int num_steps, int log_interval = 100)
{
    for (int step = 0; step < num_steps; ++step)
    {
        // 清空上一步的计算图
        global_value_pool.clear();
        
        // 1. 采样 + 构建 token 序列：BOS + encode(doc) + BOS
        string doc = data[step % data.size()];
        vector<int> tokens = {tokenizer.BOS};          // 前置 BOS
        vector<int> encoded = tokenizer.encode(doc);
        tokens.insert(tokens.end(), encoded.begin(), encoded.end());
        tokens.push_back(tokenizer.BOS);               // 后置 BOS

        // 2. 前向传播：对每个位置预测下一个 token
        vector<Vector> keys, values; // KV cache（从空开始，每个位置追加）
        Value loss(0.0);

        for (int pos = 0; pos < (int)tokens.size() - 1; ++pos)
        {
            Vector logits = model.forward(tokens[pos], pos, keys, values);
            Vector probs  = softmax(logits);
            loss = loss + (Value(0.0) - probs.data[tokens[pos + 1]].log()); // -log P(正确 token)
        }

        // 3. 平均损失 = loss / 序列长度
        Value loss_avg = loss / Value((double)(tokens.size() - 1));

        // 4. 反向传播
        loss_avg.backward();

        // 5. SGD 更新参数（在梯度算完之后立刻更新）
        auto ps = model.params();
        SGD_step(ps, /*lr=*/0.01);

        if (step % log_interval == 0)
        {
            cout << "Step " << step << ", Loss: " << loss_avg.data << endl;
        }
    }
}

// 按概率分布随机采样一个 token_id
// temperature 应在调用 softmax 前除到 logits 上；
// 这里 probs 已经是 softmax 结果，直接加权随机采样
int sample(const Vector& probs, double /*temperature*/)
{
    static mt19937 rng(42); // 固定种子，方便复现
    vector<double> p;
    for (const auto& v : probs.data)
        p.push_back(v.data);
    discrete_distribution<int> dist(p.begin(), p.end());
    return dist(rng);
}

// tokenizer 以参数传入，不放进 GPT 里——职责分离：GPT 只管向量计算
string generate(GPT& model, Tokenize& tokenizer, int max_len = 16, double temp = 0.5)
{
    vector<Vector> keys, values; // KV cache（从空开始）

    int token_id = tokenizer.BOS; // 从 BOS 开始生成
    vector<int> generated;

    for (int pos = 0; pos < max_len; ++pos)
    {
        Vector logits  = model.forward(token_id, pos, keys, values);
        Vector probs   = softmax(logits);
        token_id       = sample(probs, temp);

        if (token_id == tokenizer.BOS)
            break; // 遇到 BOS（当作 EOS）停止生成

        generated.push_back(token_id);
    }
    return tokenizer.decode(generated);
}
// 辅助函数：创建 rows×cols 的随机初始化矩阵（小随机数）
Matrix rand_matrix(int rows, int cols)
{
    static mt19937 rng(123);
    normal_distribution<double> dist(0.0, 0.02); // 均值0、标准差0.02 的小随机数
    Matrix m;
    m.row = rows;
    m.col = cols;
    m.data.resize(rows, vector<Value>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            m.data[i][j] = Value(dist(rng));
    return m;
}

int main()
{
    // ═══ 1. 读取训练数据 ═══════════════════════════════════════
    vector<string> data;
    ifstream fin("input.txt");
    if (fin.is_open()) {
        string line;
        while (getline(fin, line))
            if (!line.empty()) data.push_back(line);
        fin.close();
    }
    if (data.empty()) {
        // 没有 input.txt 就用硬编码示例（简单英文名字）
        data = {"emma", "olivia", "ava", "luna", "sophia", "mia", "harper"};
    }
    cout << "Training samples: " << data.size() << endl;

    // ═══ 2. 构建 Tokenizer（从训练数据中收集所有字符）═════════════
    set<char> charset;
    for (auto& s : data)
        for (char c : s)
            charset.insert(c);

    Tokenize tokenizer;
    for (char c : charset) {
        tokenizer.char_to_id[c] = (int)tokenizer.vocab.size();
        tokenizer.vocab.push_back(c);
    }
    tokenizer.BOS = (int)tokenizer.vocab.size(); // BOS id = 词表末尾
    tokenizer.vocab.push_back('\0');              // BOS 占位

    // ═══ 3. 更新配置 ═══════════════════════════════════════════
    cfg.vocab_size = (int)tokenizer.vocab.size();
    cout << "Vocab size: " << cfg.vocab_size << " (" << cfg.vocab_size - 1 << " chars + BOS)" << endl;

    // ═══ 4. 初始化 GPT 模型（随机权重）═══════════════════════════
    GPT model;
    model.wte     = rand_matrix(cfg.vocab_size, cfg.n_embd);  // 词表大小 × n_embd
    model.wpe     = rand_matrix(cfg.block_size, cfg.n_embd);  // 最大序列长 × n_embd
    model.lm_head = rand_matrix(cfg.vocab_size, cfg.n_embd);  // 词表大小 × n_embd

    int head_dim = cfg.n_embd / cfg.n_head;
    for (int i = 0; i < cfg.n_layer; ++i) {
        // 每层一个 AttentionBlock
        AttentionBlock attn;
        attn.n_embd   = cfg.n_embd;
        attn.n_head   = cfg.n_head;
        attn.head_dim = head_dim;
        attn.wq = rand_matrix(cfg.n_embd, cfg.n_embd);
        attn.wk = rand_matrix(cfg.n_embd, cfg.n_embd);
        attn.wv = rand_matrix(cfg.n_embd, cfg.n_embd);
        model.attn_blocks.push_back(attn);

        // 每层一个 MLPBlock
        MLPBlock mlp;
        mlp.n_embd   = cfg.n_embd;
        mlp.n_hidden = cfg.n_hidden;
        mlp.w1 = rand_matrix(cfg.n_hidden, cfg.n_embd);  // n_embd → n_hidden
        mlp.w2 = rand_matrix(cfg.n_embd, cfg.n_hidden);  // n_hidden → n_embd
        model.mlp_blocks.push_back(mlp);
    }
    cout << "Model initialized: " << cfg.n_layer << " layers, "
         << cfg.n_embd << " dim, " << cfg.n_head << " heads" << endl;

    // ═══ 5. 训练 ═══════════════════════════════════════════════
    int num_steps = 200;
    cout << "\nStarting training for " << num_steps << " steps..." << endl;
    train(model, data, tokenizer, num_steps, /*log_interval=*/10);

    // ═══ 6. 生成示例 ═══════════════════════════════════════════
    cout << "\n=== Generated samples ===" << endl;
    for (int i = 0; i < 5; ++i) {
        string result = generate(model, tokenizer, cfg.block_size, 0.8);
        cout << "  [" << i << "] " << result << endl;
    }

    return 0;
}