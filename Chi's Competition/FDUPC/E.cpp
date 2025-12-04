#include<iostream>
#include<algorithm>
#include<queue>
#include<tuple>
using namespace std;
static inline tuple<int,int,int> operator+(const tuple<int,int,int>&a,const tuple<int,int,int>& b)
{
    return {get<0>(a)+get<0>(b),get<1>(a)+get<1>(b),get<2>(a)+get<2>(b)};
}

const int N=104;
char mp[N][N];
bool tag[N][N];
bool vst[N][N];
int n,m;
tuple<int,int,int>delta[4]={{1,0,1},{-1,0,1},{0,1,1},{0,-1,1}};
int solve(int x,int y)
{
    // clear visited
    std::fill(&vst[0][0], &vst[0][0] + N * N, false);
    if (tag[x][y])return 0;
    if (mp[x][y] == '#')return -1;
    queue<tuple<int,int,int>>q;
    q.emplace(x,y,0);
    vst[x][y]=true;
    while(!q.empty()) {
        auto cur=q.front();q.pop();
        if(tag[get<0>(cur)][get<1>(cur)]) return get<2>(cur);
        for(int i=0; i<4;++i) {
            auto nxt =cur+delta[i];
            int nx=get<0>(nxt);
            int ny=get<1>(nxt);
            if (nx<1||nx>n||ny<1||ny>m) continue;
            if (mp[nx][ny]=='#') continue;
            if(vst[nx][ny])continue;
            if(tag[nx][ny])return get<2>(nxt);
            vst[nx][ny]=true;
            q.push(nxt);
        }
    }
    return -1;
}
int f[N][N];
void update(int x,int y)
{
    fill(&vst[0][0], &vst[0][0] + N * N, false);
    tag[x][y]=1;
    f[x][y]=0;
    queue<tuple<int,int,int>>q;
    q.emplace(x,y,0);
    vst[x][y]=true;
    while(!q.empty()) {
        auto cur=q.front();q.pop();
        for(int i=0;i<4;++i) {
            auto nxt = cur + delta[i];
            int nx=get<0>(nxt);
            int ny=get<1>(nxt);
            if(nx<1||nx>n||ny<1||ny>m)continue;
            if(mp[nx][ny]=='#')continue;
            if(vst[nx][ny])continue;
            vst[nx][ny]=true;
            if(f[nx][ny]==-1||f[nx][ny]>get<2>(nxt))
            {
                f[nx][ny]=get<2>(nxt);
            }
            q.push(nxt);
        }
    }
}
int main()
{
    cin>>n>>m;
    for(int i=1;i<=n;++i)
    {
        for(int j=1;j<=m;++j)
        {
            cin>>mp[i][j];
            f[i][j]=-1;
        }
    }
    int Q;cin>>Q;
    for(int k=1;k<=Q;++k)
    {
        int op,x,y;
        cin>>op>>x>>y;
        if(k>=Q)
        {
            switch(op)
            {
                case 1:
                    tag[x][y]=1;
                    break;
                case 2:
                    cout<<solve(x,y)<<endl;
                    break;
            }
        }
        else
        {
            switch(op)
            {
                case 1:
                    update(x,y);
                    break;
                case 2:
                    cout<<f[x][y]<<endl;
                    break;
            }
        }
    }
    return 0;
}