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
    fin.ignore(); 
    vector<string> content;  //后N行?
    string line;
    int lines=0; //用于计数 统计读了多少行
    // for(int i=0;i<N;i++)
    // {
    //     if(lines>1000-N-1)
    //         break;
    //     getline(fin,line);    //之前错 fin读过的getline不会再读了
    //     content.push_back(line);
    //     lines++;
    // }
/*
 *******************************************************
#   为什么要for循环？？还以N-1为界
 *******************************************************

*/
    while(getline(fin,line)&&lines<1000-N-1)
    {
        content.push_back(line);
        ++lines;
    }
    cout<<content[0]<<endl<<content[1]<<endl;
    cout<<"****"<<endl;
    //提取宝藏
    vector<string> treasure(N);
    for(int i=0;i<N;i++)
    {
        int row=data[i][0];
        int array=data[i][1]-1;
        int len=data[i][2];
        string result="";//好的初始化
        cout<<endl<<content[row-N-1]<<endl;
/*
 *******************************************************
 *  NOTE: 行编号说明 (仔细想想每个变量的含义)
 *  - 前面 N 占一行 (header)
 *  - N 行提示占 N 行 (hints)
 *  - 共占 N+1 行 (header + N lines)
 *  - 新的内容“行”标从 0 开始 (content indices are 0-based)
 *  - 自然语言“行”从 1 开始 (row is 1-based)
 *  Date: 2025-11-26
 *******************************************************
 */
        for(int k=0;k<len;k++)
        {
            int index=k+array;
       //     bool rowRange= !( row>1000-1-N || row>N+1+N || row<N+1 ) ;
            bool rowRange= !( row>lines+N+1 || row<N+1 ) ;
     //       bool charRange=rowRange && index>=0 && index <content[row-N-1].size();
   
/*
 *******************************************************
 *  TODO (仔细想想每个变量的含义) - OWNER: your-name
 *  - row: 自然语言行号 (从1开始)
 *  - rowIndex: content 索引 (从0开始) ...
 *  Date: 2025-11-26
 *******************************************************
 */
            
            if(rowRange)
            {
                bool charRange=index>=0&&index<content[row-N-2].size();

                if(i==N-1)
                {
          /*       test
           cout<<content[row-N-1].size()<<endl;
                cout<<rowRange<<" "<<charRange<<" "<<row<<" "<<N<<" "<<lines<<" "
                         <<index<<" "<<content.size()<<" "
                         <<content[row-N-1].size()<<endl;
                         */
                }
                
                if(charRange&&content[row-N-2][index]!='\r')result+=content[row-N-2][index];
            //Here was a bug: content[row-N-2][index] should be content[row-N-1][index]
                else result+="*";
                
            }
                
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