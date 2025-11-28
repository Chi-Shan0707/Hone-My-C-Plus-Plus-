#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    char dirctions[] = {'N', 'E', 'S', 'W'};
    int index=0;
    int n;
    char current_dir;
    char rotate;
    cin>>current_dir>>n;
    switch (current_dir){
        case 'N': index=0; break;
        case 'E': index=1; break;
        case 'S': index=2; break;
        case 'W': index=3; break;
    }
    for(int i=0;i<n;i++)
    {
        cin>>rotate;
        if(rotate=='L'){
            index=(index+3)%4;
        }
        else if(rotate=='R'){
            index=(index+1)%4;
        }
        else
        {
            index=(index+2)%4;
        }
        cout<<dirctions[index]<<' ';
    }
    return 0;
}