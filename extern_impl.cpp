#include <iostream>
using namespace std;

// 定义全局变量
int globalCounter = 0;
const int globalConst = 42;

// 定义函数
void incrementCounter() {
    globalCounter++;
}

void printValues() {
    cout << "globalCounter = " << globalCounter << endl;
    cout << "globalConst = " << globalConst << endl;
}