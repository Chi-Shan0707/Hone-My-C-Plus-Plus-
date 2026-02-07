#include<iostream>
void swap(int &u,int &v)
{
    int temp=u;
    u=v;
    v=temp;
}
void quick_select(int arr[], int L, int R, int m)
{

//  将m个最小的堆到左侧
    if(L==R)return;
    if(R-L+1==m)return;
    int key=arr[L];
    int lptr=L,rptr=R;//left_pointer,right_pointer
    while(lptr<rptr)
    {
        while(lptr<rptr&&key<=arr[rptr])--rptr;
        swap(arr[lptr],arr[rptr]);
        while(lptr<rptr&&arr[lptr]<=key)++lptr;
        swap(arr[lptr],arr[rptr]);
    }
    if(m<=lptr)
    {
//[L,R]前m小的肯定在lptr左侧
        quick_select(arr,L,lptr,m);
    }
    else
    {
//[L,R]前m小的，一部分已经划到[L,lptr]上了，另一部分在[lptr+1,R]
        quick_select(arr,rptr+1,R,m-lptr);
    }
}
int main()
{
    int n,m;
    std::cin>>n>>m;
    //找到前m大的数
    int *a=new int[n+1];
    for(int i=1;i<=n;i++)std::cin>>a[i];
    quick_select(a,1,n,m);
    for(int i=1;i<=m;i++)std::cout<<a[i]<<" ";
    delete[] a;
    return 0;
}