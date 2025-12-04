/*
1 8 3 4 7 6 5 2
1 8 3 5 7 6 4 2
1 8 3 5 2 4 6 7
*/

#ifndef NEXT_PERMUTATION_H_
#define NEXT_PERMUTATION_H_

#include<iostream>
template<typename T>
class Array
{
    public:
        Array(int N,T *P)
        {
            this->n=N;
            p=new T [N+4];
            for(int i=1;i<=n;++i)p[i]=P[i];
        }
        ~ Array()
        {
            delete []p;
//手动清理内存，防止内存泄漏
//否则只会删去指针
        }


        bool next_permutation()
        {
        
            int i,j;
            for(i=n-1;i>=1;--i)if(p[i]<p[i+1])break;
            if(!i)return false;
            for(j=n;j>i;--j)
            {
                if(p[j]>p[i])
                {
                    swap(p[i],p[j]);
                    break;
                }
            }
            ++i;j=n;
            for(;i<=j;++i,--j)swap(p[i],p[j]);
            return true;
        }
        void print()
        {
            for(int i=1;i<=n;++i)std::cout<<p[i]<<" ";
            std::cout<<std::endl;
        }
    private:
        int n;
        T *p;
        void swap(T &x,T&y)
        {
            T z=y;
            y=x;
            x=z;
        }   
};
/*
class Function
{
    private:
        
    public:
        void swap(int &x,int &y)
        {
            int z=y;
            y=x;
            x=z;
        }
        bool next_permutation(int n,int P[ ])
        {
            int p[2002];
            int i,j;
            for(i=1;i<=n;++i)p[i]=P[i];
            for(i=n-1;i>=1;--i)if(p[i]<p[i+1])break;
            if(!i)return false;
            for(j=n;j>i;--j)
            {
                if(p[j]>p[i])
                {
                    swap(p[i],p[j]);
                    break;
                }
            }
            ++i;j=n;
            for(;i<=j;++i,--j)swap(p[i],p[j]);
            return true;
        }
};
*/

#endif 
/*
#include<iostream>
using namespace std;
const int N=1002;
int n;
int p[N];
void swap(int &x,int &y)
{
    int z=y;
    y=x;
    x=z;
}
bool next_permutation()
{
    int i,j;
    for(i=n-1;i>=1;--i)if(p[i]<p[i+1])break;
    if(!i)return false;
    for(j=n;j>i;--j)
    {
        if(p[j]>p[i])
        {
            swap(p[i],p[j]);
            break;
        }
    }
    ++i;j=n;
    for(;i<=j;++i,--j)swap(p[i],p[j]);
    return true;
}
int main()
{
    ios::sync_with_stdio(false);
    cin>>n;
    for(int i=1;i<=n;++i)p[i]=i;
    do
    {
        cout<<endl<<"----"<<endl;
        for(int i=1;i<=n;++i)cout<<p[i]<<" ";
    }while (next_permutation());
    return 0;
}
*/
