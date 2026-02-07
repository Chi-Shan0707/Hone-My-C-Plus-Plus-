#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <queue>
#include <array>
#include <algorithm> // for std::fill

// 字符集大小 (假设只处理小写字母 a-z)
constexpr int ALPHABET_SIZE = 26;

class ACAutomaton {
private:
    // 存储每个模式串结束节点在 nodes 中的下标，用于最后按顺序输出结果
    std::vector<size_t> word_end_indices;
    
    // 按 BFS 顺序存储所有节点的下标，用于最后的逆序拓扑统计
    // 这是一个非常关键的优化：BFS 序本身就是 Fail 树的拓扑序
    std::vector<size_t> bfs_order;

    struct Node {
        // 使用 std::array 存储子节点下标，0 表示空/根
        std::array<size_t, ALPHABET_SIZE> children{}; 
        
        size_t fail = 0; // 失配指针 (Fail Pointer)
        
        // 统计数据：
        // match: 记录文本中匹配到该节点状态的次数（打标记）
        size_t match_count = 0; 
        
        // count: 记录这个节点作为多少个模式串的结尾（因为可能有重复模式串）
        // 如果题目保证模式串不重复，可以用 bool is_word
        size_t pattern_end_count = 0; 

        Node() { children.fill(0); }
    };

    std::vector<Node> nodes;

    // 辅助函数：字符转索引
    // [[nodiscard]] 提醒编译器检查返回值
    [[nodiscard]] int char_to_index(char c) const {
        return c - 'a';
    }

public:
    ACAutomaton() {
        nodes.emplace_back(); // 初始化根节点 (Index 0)
    }

    // 1. 插入模式串 (Insert)
    void insert(std::string_view pattern) {
        size_t curr = 0;
        for (char c : pattern) {
            int idx = char_to_index(c);
            if (nodes[curr].children[idx] == 0) {
                nodes[curr].children[idx] = nodes.size();
                nodes.emplace_back();
            }
            curr = nodes[curr].children[idx];
        }
        // 记录该模式串结束的位置
        word_end_indices.push_back(curr);
        // 标记该节点是一个模式串的结尾
        nodes[curr].pattern_end_count++; 
    }

    // 2. 构建 Fail 指针 (Build)
    void build() {
        std::queue<size_t> q;
        
        // Fail 树构建逻辑：
        // 第一层的节点 fail 指向根节点 0
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            size_t child = nodes[0].children[i];
            if (child != 0) {
                nodes[child].fail = 0;
                q.push(child);
            }
        }

        // BFS 遍历
        while (!q.empty()) {
            size_t u = q.front();
            q.pop();
            
            // 【关键】：记录 BFS 顺序，这天然就是拓扑序
            // 深度浅的节点一定先进入队列。
            // 在 Fail 树中，箭头总是指向深度更浅的节点。
            // 所以后续我们逆序遍历这个 bfs_order，就能从叶子向根汇总数据。
            bfs_order.push_back(u);

            for (int i = 0; i < ALPHABET_SIZE; ++i) {
                size_t child = nodes[u].children[i];
                
                if (child != 0) {
                    // 如果有真子节点：建立 Fail 指针
                    // 它的 Fail 指向 "父节点的 Fail 指向的节点的对应子节点"
                    nodes[child].fail = nodes[nodes[u].fail].children[i];
                    q.push(child);
                } else {
                    // 【Trie 图优化 / 路径压缩】
                    // 如果没有子节点，直接建立 "虚拟边" (Shortcut)
                    // 将这个空位置指向 "父节点 Fail 指向节点的对应子节点"
                    // 这样查询时，curr = nodes[curr].children[i] 就能自动处理失配跳转，实现 O(1) 转移
                    nodes[u].children[i] = nodes[nodes[u].fail].children[i];
                }
            }
        }
    }

    // 3. 查询文本 (Query)
    [[nodiscard]] std::vector<size_t> query(std::string_view text) {
        size_t curr = 0;
        
        // 步骤 A: 扫描文本，只打标记
        // 这一步复杂度严格是 O(Text_Length)
        for (char c : text) {
            int idx = char_to_index(c);
            // 利用 Trie 图优化，直接跳转，不需要 while 回溯
            curr = nodes[curr].children[idx];
            
            // 只在当前到达的节点打标记，不费力去爬 Fail 树
            nodes[curr].match_count++;
        }

        // 步骤 B: 按照逆序拓扑序汇总数据 (Fail 树上的动态规划)
        // 这一步复杂度是 O(Nodes_Size)
        // 从 BFS 序列的末尾往前遍历，相当于在 Fail 树上从叶子往根爬
        for (auto it = bfs_order.rbegin(); it != bfs_order.rend(); ++it) {
            size_t u = *it;
            // 将当前节点的匹配次数，累加到它的 Fail 指针指向的节点（父节点）
            // 因为匹配了 "she" (u)，也意味着匹配了 "he" (fail[u])
            nodes[nodes[u].fail].match_count += nodes[u].match_count;
        }

        // 步骤 C: 收集结果
        std::vector<size_t> results;
        results.reserve(word_end_indices.size());
        for (size_t index : word_end_indices) {
            // 结果 = 该节点的匹配次数（来自文本扫描+子节点贡献） 
            // 注意：这里不需要乘 pattern_end_count，因为题目通常问每个模式串出现了多少次
            results.push_back(nodes[index].match_count);
        }
        return results;
    }
};

int main() {
    // 优化 I/O 性能
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ACAutomaton ac;

    int n_patterns;
    if (!(std::cin >> n_patterns)) return 0;

    for (int i = 0; i < n_patterns; ++i) {
        std::string pattern;
        std::cin >> pattern;
        ac.insert(pattern);
    }

    ac.build();

    std::string text;
    std::cin >> text;

    auto results = ac.query(text);
    
    for (const auto& res : results) {
        std::cout << res << "\n";
    }

    return 0;
}