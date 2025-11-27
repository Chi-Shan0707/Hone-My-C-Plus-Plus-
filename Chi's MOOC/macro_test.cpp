#include <iostream>
using namespace std;

const int N = 100;
struct Tree_Node {
    int val, col;
    int pa;
    int child[2];
    int sz;
} tr[N];

// 定义宏
#define GET_PA(id) tr[id].pa
#define REF_PA(id) (tr[id].pa)

int main() {
    // 初始化测试节点
    for (int i = 0; i < 5; i++) {
        tr[i].pa = i;
        cout << "tr[" << i << "].pa = " << tr[i].pa << endl;
    }

    // 测试宏定义是否可以作为左值(赋值)
    cout << "\nBefore assignment: tr[1].pa = " << tr[1].pa << endl;
    
    // 使用宏作为左值进行赋值
    GET_PA(1) = 10;
    cout << "After GET_PA(1) = 10: tr[1].pa = " << tr[1].pa << endl;
    
    // 使用宏作为右值获取值
    int value = GET_PA(1);
    cout << "Value retrieved using GET_PA(1): " << value << endl;
    
    // 使用带括号的宏作为左值
    REF_PA(2) = 20;
    cout << "After REF_PA(2) = 20: tr[2].pa = " << tr[2].pa << endl;

    return 0;
}