#include <iostream>
using namespace std;

// 声明使用外部定义的变量
extern int globalCounter;
extern const int globalConst;

// 声明使用外部定义的函数
extern void incrementCounter();
extern void printValues();

int main() {
    cout << "Initial values:" << endl;
    cout << "globalCounter = " << globalCounter << endl;
    cout << "globalConst = " << globalConst << endl;
    
    cout << "\nAfter incrementing:" << endl;
    incrementCounter();
    printValues();
    
    incrementCounter();
    printValues();
    
    return 0;
}