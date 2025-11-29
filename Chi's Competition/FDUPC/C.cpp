#include<iostream>
#include<algorithm>
using namespace std;
const int N=100005;
typedef long long ll;
int main()
{
    int n;ll d[N],x[N];
    ll ans=0LL;
    cin>>n;
    for(int i=1;i<=n;++i)
    {
        ll a , b;
        cin>>a>>b;
        d[i]=a-b;
    }
    for(int i=1;i<=n;++i)cin>>x[i];
    sort(d+1,d+n+1);
    sort(x+1,x+n+1);
    for(int i=1;i<=n;++i)ans+=abs(d[i]+x[n+1-i]);
    cout<<ans<<endl;
    return 0;
}