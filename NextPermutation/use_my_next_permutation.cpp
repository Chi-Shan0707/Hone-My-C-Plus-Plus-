#include "next_permutation.h"
#include<iostream>
using namespace std;
int b[1002];
int main() {
    // 测试整数数组
    int m;
    cin>>m;
    
    for(int i=1;i<=m;++i)cin>>b[i];
    Array<int> my_array(m,b);
    do
    {
        my_array.print();
    }while (my_array.next_permutation());
    cout<<endl;
    cout<<"原数组并未被更改"<<endl;
    for(int i=1;i<=m;++i)cout<<b[i]<<" ";
    return 0;
}