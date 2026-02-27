#include<iostream>
#include<ctime>
int main()
{
    freopen("input.txt","w",stdout);
    srand(227);  // 固定种子，使随机数可复现
    for(int i=0; i< 1000; ++i)
    {
        int a = rand() % 100 ;
        int b = rand() % 100 ;
        
        std::cout << a << "+" << b << "=" << (a+b) << std::endl;
    }
    return 0;
}