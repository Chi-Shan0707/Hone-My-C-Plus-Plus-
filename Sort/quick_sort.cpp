
#include<iostream>
using namespace std;
int n,a[1003];
void swap(int &x,int &y){int z=y;y=x;x=z;}


class Array
{
    public:
        void quick_sort(int l,int r)
        {
            if(l>=r)return;
            int pivot=partition(l,r);
            quick_sort(l,pivot-1);
            quick_sort(pivot+1,r);
            for(int i=1;i<=n;++i)
            {
                cout<<a[i]<<" ";
            }
            cout<<endl<<"-------------------"<<endl;
        }
    private:
        int partition(int l,int r)
        {
            int left=l,right=r;
            while(left<right)
            {
            while(right>left&&a[right]>=a[l])--right;
            while(left<right&&a[left]<=a[l])++left;
/*
注意点：
两个while顺序不能换&&至少有一个得有取等
和left不能设置成l+1

当l=r+1的时候，如果left=l+1,大的while就不执行了
当l=r+1的时候，如果++left在前，那么就会直接left=right=r
指针需要再细细品
*/
            swap(a[left],a[right]);
            }
            swap(a[l],a[right]);
            return right;
        }
}
int main()
{
cin>>n;for(int i=1;i<=n;++i)cin>>a[i];

    return 0;
}

