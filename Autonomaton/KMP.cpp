#include<iostream>
#include<string>
#include <string_view>
#include<vector>
using namespace std;
[[nodiscard]] vector<size_t> compute_pi(string_view pattern) 
{
    size_t m = pattern.length();
    if (m == 0) return {};

    // 使用 vector 管理内存，初始化为 0
    vector<size_t> pi(m, 0);

    pi[0] = 0; // patter[0-0] 的前缀函数值为 0
    
    for (size_t i = 1, j = 0; i < m; ++i)
    {
        
        //考虑的是pattern.substr(0,i+1)的前缀后缀匹配情况

        //j 代表当前匹配的前缀长度
        
        if (pattern[i] == pattern[j])
        {
            ++j;
        }
        else
        {
            while(j>0)
            {
                j = pi[j-1];
                //如果不匹配，回退 j (注意 j>0 的判断，避免 size_t 下溢)
                if ( j == 0)break;
                if (pattern[i] == pattern[j])
                {
                    break;
                }

            }
            if (pattern[i] == pattern[j])
            {
                ++j;
            }
        }
        pi[i] = j;
    }
    return pi;
}
   /*
  核心概念：零拷贝 (Zero-Copy)
用最通俗的话解释：

std::string (老派)：当你把一个字符串传给函数时，如果用值传递，它会复印一份完整的副本（申请内存 + 复制数据）。这很慢。

const std::string& (改进)：这是引用，不用复印，但它强制要求对方必须是一个 std::string 对象。如果你传的是 "hello" (C 风格字符串)，编译器会被迫在后台偷偷创建一个临时的 std::string 对象（依然要申请内存）。

std::string_view (现代)：它仅仅保存了两个极其轻量的值：

字符串的首地址指针 (ptr)

字符串的长度 (len)

它不拥有字符串，不申请内存，只是“看”着一块现有的内存。
     */


// 2. KMP 搜索主函数
// 返回所有匹配项的起始索引
vector<size_t> kmp_search(string_view text, string_view pattern) {
    vector<size_t> matches;
    size_t n = text.length();
    size_t m = pattern.length();

    if (m == 0 || n < m) return matches;

    // 获取 Pi 数组
    vector<size_t> pi = compute_pi(pattern);

    // j 代表当前 pattern 匹配到的位置
    for (size_t i = 0, j = 0; i < n; ++i) {
        // 不匹配时的回退逻辑
        while (j > 0 && text[i] != pattern[j]) {
            j = pi[j - 1];
        }

        // 匹配成功，推进 pattern 指针
        if (text[i] == pattern[j]) {
            j++;
        }

        // 完整匹配整个 pattern
        if (j == m) {
            // 记录匹配位置 (i 结尾，长度 m，所以起始是 i - m + 1)
            matches.push_back(i - m + 1);
            
            // 继续寻找下一个匹配，利用 pi 数组回退
            j = pi[j - 1]; 
        }
    }
    return matches;
}

// 3. 采用拼接字符串的方式
vector<size_t> kmp_piece(const string&text, const string&pattern)
{

    vector<size_t> matches;
    string str = pattern + "|" + text;
    vector<size_t> pi = compute_pi(str);
    for (size_t i = pattern.length() + 1; i < str.length(); ++i) 
    {
        if (pi[i] == pattern.length())
        {
            matches.push_back(i - 2 * pattern.length());
        }
    }
    return matches;
}
int main()
{
    ios::sync_with_stdio(false);
    //Text: 文本串
    string t;
    //Pattern: 模式串
    string p;
    cin>>t>>p;
    
    
    vector<size_t> matches = kmp_piece(t, p);
    for (size_t idx : matches)cout << idx+1 << endl;//matches的idx是从0开始算的
    
    vector<size_t> pi = compute_pi(p);
    for (size_t i = 0; i < pi.size(); ++i) cout<<pi[i]<<" ";
    return 0;
}