#include<iostream>
#include "quick_sort.h"
using namespace std;
int main()
{
    int n;
    cout << "请输入元素个数: ";
    cout.flush();
    cin>>n;
    int a[1002];
    cout << "请输入" << n << "个数字: ";
    cout.flush();
    for(int i=1;i<=n;i++)cin>>a[i];
    Sort<int> my_quick_sort;
    my_quick_sort.quick_sort(a,1,n);
    cout << "排序结果: ";
    for (int  i=1;i<=n;++i)cout<<a[i]<<" ";
    cout << endl;
    return 0;
}

/*
1. 检查 launch.json 配置
确保您的 .vscode/launch.json 配置正确：


externalconsole true与false的区别
*/

/*
bash终端里ERROR: Unable to start debugging. Unexpected GDB output from command "-exec-run". During startup program exited with code 0xc0000139.
The program 'D:\C_C++\Hone_My_C++\Chi's Binary_Search_Tree\fhq_Treap.exe' has exited with code 0 (0x00000000). 已知是cppdbg模式，但c程序能正确运行；在换成bash终端前，c++也能正确运行；我的程序#include<iostream>
int main()
{
    int d;
    std::cin>>d;
    return 0;
}简单如这样也会直接崩溃，虽然bash那边是蓝色的显示编译成功


2. 切换终端类型
在 VS Code 中尝试：

按 Ctrl+Shift+P

输入 "Terminal: Select Default Profile"

选择 "Command Prompt" 而不是 "Git Bash"

重启终端和 VS Code
*/