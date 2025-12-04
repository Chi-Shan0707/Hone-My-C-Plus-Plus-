#include<iostream>
using namespace std;
int main()
{
    bool get_floor=false,get_height=false;
    int floor=1,height=1;
    bool state;
    while(!get_floor)
    {
        cout<<"? "<<0<<endl;
        cout<<flush;
        cin>>state;
        floor+=state;
        get_floor=(!state);
    }
    while(!get_height)
    {
        cout<<"? "<<1<<endl;
        cout<<flush;
        cin>>state;
        height+=state;
        get_height=(!state);
    }
    for(int i=height;i>floor;--i)
    {
        cout<<"? "<<0<<endl;
        cout<<flush;
        cin>>state;
    }
    cout<<"! "<<height<<" "<<floor<<endl;
    cout<<flush;
    return 0;
}
//4 3
//3 1