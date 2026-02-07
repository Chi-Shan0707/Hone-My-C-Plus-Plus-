#include<iostream>
#include<vector>
#include<numeric>
void try_reference(int &Nickname)
{
    
}
int main()
{
    std::vector<int>vec={0,1,2,3};
    for(auto i:vec)
    {
        std::cout<<i<<" ";
    }
    std::cout<<std::endl;
    for(const auto &i:vec)
    {
        std::cout<<i<<" ";
    }
    std::cout<<std::endl;
    int sum=std::accumulate(vec.begin(),vec.end(),0);
    std::cout<<sum<<std::endl;



    int n=10;
    return 0;
}