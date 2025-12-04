#include<bits/stdc++.h>
using namespace std;
int main(){
    string s;
    getline(cin,s);
    string t="TuLingJiang";
    int n=s.size();
    int m=t.size();
    for(int i=0;i<n;i++){
        cout<<s[i];
        if(i<m-1)continue;
        bool flag=true;
        for(int j=0;j<m;++j)
        {
            if(s[i-m+j+1]!=t[j])
            {
                flag=false;
                break;
            }
        }
        if(flag){
            cout<<"~";
            
        }
    }
    return 0;
}