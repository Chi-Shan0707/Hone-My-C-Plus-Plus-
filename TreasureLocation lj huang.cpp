#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

int main()
{
    ifstream fin;
    fin.open("clues_and_plays.txt");
    if(!fin)
        return 0;
    
    //读取
    int N;
    fin>>N;
    vector<vector<int>> data(N,vector<int>(3));  //前N+1行
    for(int i=0;i<N;i++)
        fin>>data[i][0]>>data[i][1]>>data[i][2];
    fin.ignore();  //修正：忽略之前读取残留的换行符
    vector<string> content;  //后N行
    string line;
    int lines=0; //用于计数 统计读了多少行
    for(int i=0;i<N;i++)
    {
        if(lines>1000-N-1)
            break;
        getline(fin,line);    //之前错 fin读过的getline不会再读了
        content.push_back(line);
        lines++;
    }
    //提取宝藏
    vector<string> treasure(N);
    for(int i=0;i<N;i++)
    {
        int row=data[i][0];
        int array=data[i][1]-1;
        int len=data[i][2];
        string result;
        for(int k=0;k<len;k++)
        {
            int index=k+array;
            bool rowRange= !( row>1000-1-N || row>N+1+N || row<N+1 ) ;
            bool charRange=rowRange && index>=0 && index <content[row-N-2].size();
            if(charRange)
                result+=content[row-N-2][index];
            else
                result+='*';
        }
        treasure[i]=result;
    }
    fin.close();

    ofstream fout;
    fout.open("secret_message.txt");
    for(int i=0;i<N-1;i++)
        fout<<treasure[i]<<' ';
    fout<<treasure[N-1];

    fout.close();
    return 0;
}