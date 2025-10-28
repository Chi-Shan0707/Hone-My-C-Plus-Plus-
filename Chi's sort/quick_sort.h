
#ifndef QUICK_SORT_H
#define QUICK_SORT_H
using namespace std;
template<typename T>
void swap(T &x,T &y){T z=y;y=x;x=z;}
//template<typename T>只能被用一次，有东东用掉了就得再写一遍

template<typename T>
class Sort
{
//    template<typename T>;
    public:
        void quick_sort(T a[],int l,int r)
        {
            if(l>=r)return;
            int pivot=partition(a,l,r);
            quick_sort(a,l,pivot-1);
            quick_sort(a,pivot+1,r);
        }
    private:
        void swap(T &u,T &v)
        {
            T temp=u;
            u=v;
            v=temp;
        }
        int partition(T a[],int l,int r)
        {
            int left=l,right=r;
            T key=a[left];
            while(left<right)
            {
                //先考虑右侧≥key
                while(right>left&&a[right]>=key)--right;
                swap(a[left],a[right]);

                //再考虑左侧≤key
                while(left<right&&a[left]<=key)++left;
                swap(a[left],a[right]);
        /*
        很精妙的算法，当在第一个while的时候，key在left上，当right指向一个比较小的数的时候，就会互调leftright，开启第二个while，此时key在right上，移动left看left侧的值有没有≤key的，如果有，则再次与right交换，使得这个较大的值变到key右侧
        */
            }
            return right;//此时left==right
        }
};


/*
int partition(int a[],int l,int r)
        {
            int left=l,right=r;
            while(left<right)
            {
                while(right>left&&a[right]>=a[l])--right;
                while(left<right&&a[left]<=a[l])++left;

注意点：
两个while顺序不能换&&至少有一个得有取等
和left不能设置成l+1

当l=r+1的时候，如果left=l+1,大的while就不执行了
当l=r+1的时候，如果++left在前，那么就会直接left=right=r
指针需要再细细品

                swap(a[left],a[right]);
            }
            swap(a[l],a[right]);
            return right;
        }

*/

#endif

