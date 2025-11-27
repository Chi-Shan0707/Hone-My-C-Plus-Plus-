#include <iostream>
#include <sstream>

using namespace std;
/*
int stringToInteger(const string& s) {
    istringstream iss(s);

    int result;
    if (!(iss >> result))
        throw std::domain_error("stringToInteger: no valid integer at beginning found");

    char remain;
    if (iss >> remain)
        throw std::domain_error("stringToInteger: can't have characters after the int");
    return result;
}
*/
void basic_try()
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
}
int input_integer(const string&str)
{
    istringstream iss(str);//初始化于缓冲区
    int res;
    iss>>res;
    return res;
}
int main()
{
    string number;
    std::cin>>number;
    cout<<input_integer(number)<<endl;
    return 0;
}