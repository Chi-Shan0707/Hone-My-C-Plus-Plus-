#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    istringstream iss("Initialize istringstream");
//position at the front
    ostringstream oss("Initial ostringstream",stringstream::ate);
//position at the end
    std::cout<<oss.str()<<endl;
    string INPUT;
    iss>>INPUT;
    //遇到空格自己断
    cout<<INPUT<<endl;
    oss<<INPUT;
    cout<<oss.str()<<endl;
    cout<<iss.str()<<endl;
    return 0;
}