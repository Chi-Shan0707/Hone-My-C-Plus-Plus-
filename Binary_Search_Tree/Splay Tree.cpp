/*
1. 每个节点红色或黑色
2. 根节点为黑色
3. 叶子节点为null，且为黑色
4. 不存在两个相邻的红色节点
5. 从任意节点往下走到叶子节点（为null）的路径上，有同样数目的黑色节点
*/


#include<iostream>
using namespace std;
#define LEFT 0
#define RIGHT 1
const int N=100005;
int n,tot,root;
struct Tree_Node
{
    int val;
    int pa;
    int child[2];//left,right
    int cnt,sz;
}tr[N];

#define sz(id) tr[id].sz

#define Pa(id) tr[id].pa
/*
宏定义本质上是在编译前进行文本替换，
所以 PA(id)（即 tr[id].pa）是左值，可以对其进行赋值操作。
*/
void print_info()
{
    cout<<"____"<<endl;
 // cout<<"根节点"<<root<<endl;
    for(int j=1;j<=tot;++j)

        {
            cout<<"节点"<<j<<" val:"<<tr[j].val<<" "<<tr[j].cnt<<" "<<tr[j].sz<<" sons"<<tr[j].child[LEFT]<<" "<<tr[j].child[RIGHT]<<" "<<tr[j].pa<<endl;
        }
        cout<<"____"<<endl;
}
void push_up(int id){
    sz(id)=sz(tr[id].child[LEFT])+sz(tr[id].child[RIGHT])+tr[id].cnt;


 //   cout<<"更新id"<<endl;
//   print_info();
}
void clockwise_rotate(int id)
{
//closewise顺时针
    int parent=Pa(id);

  //每一步操作都要注意，双向关系的维护   
    //先搞定原爷爷
    if(tr[Pa(parent)].child[LEFT]==parent)tr[Pa(parent)].child[LEFT]=id;
    else tr[Pa(parent)].child[RIGHT]=id;
    Pa(id)=Pa(parent);

    //再搞定自己的儿子
    
    tr[parent].child[LEFT]=tr[id].child[RIGHT];
    Pa(tr[id].child[RIGHT])=parent;
    
    //最后搞定自己的父亲
    Pa(parent)=id;
    tr[id].child[RIGHT]=parent;

    push_up(parent);
    push_up(id);
}
void anticlockwise_rotate(int id)
{
//anticlosewise逆时针
    int parent=Pa(id);

  //每一步操作都要注意，双向关系的维护   
    //先搞定原爷爷
    if(tr[Pa(parent)].child[LEFT]==parent)tr[Pa(parent)].child[LEFT]=id;
    else tr[Pa(parent)].child[RIGHT]=id;
    Pa(id)=Pa(parent);

    //再搞定自己的儿子
    
    tr[parent].child[RIGHT]=tr[id].child[LEFT];
    Pa(tr[id].child[LEFT])=parent;
    
    //最后搞定自己的父亲
    Pa(parent)=id;
    tr[id].child[LEFT]=parent;

    push_up(parent);
    push_up(id);
}
void splay(int &root,int id)
{
    if(id==root)return;
    for(;Pa(id)!=0;)
    //id会自己递变，通过id的父亲?0来判断是否到达根节点
    //不能以id!=root 来当作判断语句
    {
        int parent=Pa(id);
//       cout<<"id/parent"<<id<<" "<<parent<<endl;
        if(parent==root)
        {
            if(tr[parent].child[LEFT]==id)clockwise_rotate(id);
            else anticlockwise_rotate(id);
            root=id;
            return;
        }
        else{
            if(tr[Pa(parent)].child[LEFT]==parent)
            {
                if(tr[parent].child[LEFT]==id)
                {
                    clockwise_rotate(parent);
                    clockwise_rotate(id);
                }
                else
                {
 //                   cout<<"TYPE2"<<endl;
            //        print_info();
                    anticlockwise_rotate(id);
                 //   print_info();
                    clockwise_rotate(id);
                  //  print_info();

                }
            }
            else
            {
                if(tr[parent].child[LEFT]==id)
                {
                    
                    clockwise_rotate(id);
                    anticlockwise_rotate(id);
                }
                else
                {
                    
                    anticlockwise_rotate(parent);
                    anticlockwise_rotate(id);
                }
            }
            
        }
    }
    root=id;
}
void insert(int &root,int val)
{
//    cout<<"当前root"<<root<<"值"<<tr[root].val<<endl;
    int id=root;
    int parent=0;
    for(;id&&val!=tr[id].val;id=tr[parent=id].child[tr[id].val<val]);
//跨越到下一步的时候记录父亲
//tr[id].val<val为真：RIGHT
    if(id)
    {
        ++tr[id].cnt;
        ++tr[id].sz;
    }
    else
    {
        
        tr[id=++tot]=(Tree_Node){val
                                ,parent,
                                {0,0},
                                1,1};
        
        if(tot==1)
        {
            root=1;
            return;
        }
        tr[parent].child[tr[parent].val<val]=id;
//        cout<<"新的来了"<<endl;
//        print_info();
//        cout<<"新节点"<<id<<" "<<tot<<"   -- "<<parent<<endl;
    }
    splay(root,id);
}

int Rank(int &root,int val)
{
    int id=root;
    for(;tr[id].val!=val;id=tr[id].child[tr[id].val<val]);
    splay(root,id);
    return sz(tr[root].child[LEFT])+1;
}
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;++i)
    {
        int num;cin>>num;
        insert(root,num);
//        cout<<endl<<"插入完毕"<<endl;
        /*
        Tree_Node new_node;
        new_node={num,RED,
                    0,
                    {0,0},
                  1,1};
                  */
     //   cout<<"统计"<<endl;
 //       print_info();
        cout<<"名次"<<Rank(root,num)<<endl;
     //   print_info();
}
    return 0;
}