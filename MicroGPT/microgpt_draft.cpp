#include <iostream>
#include <locale>
#include <string>
#include <set>
#include <cwctype> 
#include <vector>
#include <cmath>
using namespace std;


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
    // c = a + b
        Value result;
        result.data = this->data + other.data;
        result.children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result.local_grads = {1.0, 1.0}; // dc/da = 1, dc/db = 1
        return result;
    }
    Value operator*(const Value& other) const {
    // c = a * b
        Value result;
        result.data = this->data * other.data;
        result.children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result.local_grads = {other.data, this->data}; // dc/da = b, dc/db = a
        return result; 
    }
    Value operator-(const Value& other) const {
    // c = a - b
        Value result;
        result.data = this->data - other.data;  
        result.children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result.local_grads = {1.0, -1.0}; // dc/da = 1, dc/db = -1
        return result;
    }
    Value operator/(const Value& other) const {
    // c = a / b
        Value result;
        result.data = this->data / other.data;
        result.children = {const_cast<Value*>(this), const_cast<Value*>(&other)};
        result.local_grads = {1.0 / other.data, -this->data /
                            (other.data * other.data)}; // dc/da = 1/b, dc/db = -a/b^2
        return result;
    }

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

Vector linear(const Vector& input, const Matrix& weights)
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
int main()
{
    // 设置全局 locale 为 UTF-8 以支持宽字符 I/O
    locale::global(locale("C.utf8"));
    // ** 这个因不同平台不同电脑而异，可能需要调整为 "en_US.UTF-8" 或其他适合的 UTF-8 locale 名称 **

    
    wcin.imbue(locale());
    wcout.imbue(locale());

    // 使用 set<wchar_t> 存储唯一的 Unicode 码点（字符）
    set<wchar_t> u_chars;
    set<wchar_t> ::iterator it;
    wstring line;
    while (getline(wcin, line)) 
    {
        for (auto const & ch : line) 
        {
            u_chars.insert(ch);
        }
    }
    


/*
    // 示例：打印出收集到的字符数量和这些字符
    wcout << L"Unique characters collected: " << u_chars.size() << endl;
    for (auto const & ch : u_chars) {
        wcout << ch;
    }
    wcout << endl;
*/
    
    vector<wchar_t> u_chars_vec(u_chars.begin(), u_chars.end());
    wcout << L"Unique characters collected: " << u_chars_vec.size() << endl;
    auto BOS = u_chars_vec.size(); // 以当前字符数量作为 BOS 的索引
    

    return 0;
}