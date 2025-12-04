#include<iostream>
#include<vector>
std::vector<int>minheap;
int tot;
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
int left_child(int id) {
    return (id<<1)^1;
}
int right_child(int id) {
    return (id+1)<<1;
}
int parent(int id)
{
    return (id-1)>>1;
}
int peak()
{
    return minheap[0];
}
void siftUp(int id)
{
    while (true) {
        // 获取节点 i 的父节点
        int p = parent(id);
        // 当“越过根节点”或“节点无须修复”时，结束堆化
        if (p < 0 || minheap[id] >= minheap[p])
            break;
        // 交换两节点
       swap(minheap[id], minheap[p]);
        // 循环向上堆化
        id = p;
    }
}
/* 元素入堆 */
void push(int val) {
    // 添加节点
    ++tot;
    minheap.push_back(val);
    // 从底至顶堆化
    siftUp(tot-1);
}

/* 从节点 i 开始，从底至顶堆化 */

int main()
{
    
    return 0;
}