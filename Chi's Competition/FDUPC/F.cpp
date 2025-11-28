#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
//思路10^9值域比较
ull c[3002][3002];
inline ull solve(int n,int m,ull val)
{
    if(val==0LL)return 0LL;
    ull res=0LL;
    if(n<=3000)
    {
        int low,high,mid,pos;
        for(int i=1;i<=n;++i)
        {
            if(c[i][(i>>1)]<=val)
            {
                res+=(m+1);
                continue;
            }
            low=0,high=(i>>1),mid=0,pos=0;
            while(low<=high)
            {
                mid=(low+high)>>1;
                if(c[i][mid]<=val)
                {
                    pos=mid;
                    low=mid+1;
                }
                else
                {
                    high=mid-1;
                }
            }
            if(m<(i>>1))
            {
                res+=(min((ull)pos,(ull)m)+1LL);
            }
            else
            {
                res+=(pos+1LL);
                if(i-pos<=m)
                {
                    res+=(m-(i-pos)+1LL);
                    
                }
            }
        }
        
    }
    else
    {
        for(int i=1;i<=3000;++i)
        {
            if(c[i][(i>>1)]<=val)
            {
                res+=(m+1);
                continue;
            }
            int low,high,mid,pos;
            low=0,high=(i>>1),mid=0,pos=0;
            while(low<=high)
            {
                mid=(low+high)>>1;
                if(c[i][mid]<=val)
                {
                    pos=mid;
                    low=mid+1;
                }
                else
                {
                    high=mid-1;
                }
            }
            if(m<(i>>1))
            {
                res+=(min((ull)pos,(ull)m)+1LL);
            }
            else
            {
                res+=(pos+1LL);
                if(i-pos<=m)
                {
                    res+=(m-(i-pos)+1LL);
                    
                }
            }
        }
        
//先看0
        res+=(n-3000);
        if(m>3000)res+=(m-3000);
//再看1
        if(m==0)return res;
        ull lmt;
        if(val>3000)
        {
           lmt=min((ull)n,val);
           res+=(lmt-3000);
           if(lmt<=(m+1))
           {
                res+=(lmt-3000);
           }
           else if(3000<(m+1))
           {
                res+=((m+1)-3000);
           }
        }
        else
        {
            return res;
        }
        if(m==1)return res;
//再看2
        lmt=(1+int(sqrt(1+(val<<3))))>>1;
        lmt=min((ull)n,lmt);
        if(lmt<=3000)return res;
        res+=(lmt-3000);
        if(lmt<=(m+2))
        {
            res+=(lmt-3000);
        }
        else if(3000<(m+2))
        {
            res+=((m+2)-3000);
        }
    }
    return res;
}
/*
        分值域操作
*/
int main()
{
    ios::sync_with_stdio(false);
    for(int i=0;i<=3000;i++)
    {
        c[i][0]=1;
        for(int j=1;j<=i;j++)
        {
            c[i][j]=c[i-1][j-1]+c[i-1][j];
        }
    }
    int q;
    cin>>q;
    while(q--)
    {
        int n,m;
        ull l,r;
        cin>>n>>m>>l>>r;
        m=min(m,n);
        cout<<solve(n,m,r)-solve(n,m,l-1LL)<<endl;
    }
    return 0;
}