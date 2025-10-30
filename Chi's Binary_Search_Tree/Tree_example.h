#ifndef TREE_EXAMPLE_H_
#define TREE_EXAMPLE_H_

struct Tree_Node
{
    int val;
    int pa;
    int child[2]; // left,right
    int cnt, sz;
};

#define LEFT 0
#define RIGHT 1

class Binary_Search_Tree
{
private:
    Tree_Node* tr;  // 成员变量指针
    int capacity;   // 记录容量

public:
    // 构造函数 - 分配内存
    Binary_Search_Tree(int n)
    {
        tr = new Tree_Node[n];
        capacity = n;
        
        // 初始化节点
        for (int i = 0; i < n; i++) {
            tr[i].val = 0;
            tr[i].pa = 0;
            tr[i].child[LEFT] = 0;
            tr[i].child[RIGHT] = 0;
            tr[i].cnt = 0;
            tr[i].sz = 0;
        }
    }
    
    // 析构函数 - 释放内存
    ~Binary_Search_Tree()
    {
        delete[] tr;  // 释放数组内存
        tr = nullptr; // 避免悬空指针
    }
    
    // 其他成员函数...
    void push_up(int id) {
        // 示例实现
    }
    
    int which_one(int id) {
        return 0; // 示例实现
    }
    
    void rotate(int id) {
        // 示例实现
    }
};

#endif