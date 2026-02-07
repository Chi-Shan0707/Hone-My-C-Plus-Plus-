#include <iostream>
using namespace std;

int main() {
    // 创建一个数组
    int originalArray[] = {1, 2, 3, 4, 5};
    int* P = originalArray;  // P指向原始数组
    
    cout << "原始数组地址: " << originalArray << endl;
    cout << "P的值(指向的地址): " << P << endl;
    cout << "P本身的地址: " << &P << endl;
    
    // 模拟你的代码中的情况
    int* p;  // 类内部的指针
    p = new int[5];  // 为p分配新内存
    
    cout << "\n为p分配内存后:" << endl;
    cout << "p的值(指向的地址): " << p << endl;
    cout << "p本身的地址: " << &p << endl;
    
    // 将P指向的数据复制到p指向的内存中
    for(int i = 0; i < 5; i++) {
        p[i] = P[i];
    }
    
    cout << "\n复制数据后:" << endl;
    cout << "P[0] = " << P[0] << ", p[0] = " << p[0] << endl;
    
    // 修改p指向的数据
    p[0] = 999;
    
    cout << "\n修改p[0]后:" << endl;
    cout << "P[0] = " << P[0] << ", p[0] = " << p[0] << endl;
    
    // 清理内存
    delete[] p;
    
    return 0;
}