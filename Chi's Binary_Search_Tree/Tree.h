#ifndef TREE_H_
#define TREE_H

struct Tree_Node
{
    int val;
    int pa;
    int child[2];//left,right
    int cnt,sz;
};
#define LEFT 0
#define RIGHT 1
#define sz(id) tr[id].sz
#define Pa(id) tr[id].pa
class Binary_Search_Tree
{
    private:

    public:
        Tree_Node *tr;  // 将tr声明为类成员变量
        int root,node_tot;
        void push_up(int id){sz(id)=sz(tr[id].child[LEFT])+sz(tr[id].child[RIGHT])+tr[id].cnt;}
        void update(int id)
        {
            push_up(id);
            if(id==root)return;
            update(tr[id].pa);
        }
        int which_one(int id){return tr[Pa(id)].child[RIGHT]==id;}
        Binary_Search_Tree(int n)
        {
            tr=new Tree_Node[n];//动态数组
            root=node_tot=0;
//初始化


            for(int i=0;i<n;++i)tr[i].val=tr[i].pa=tr[i].child[LEFT]=tr[i].child[RIGHT]=tr[i].cnt=tr[i].sz=0;
        }
        ~Binary_Search_Tree()
        {
//new的东东要靠delete，这里指针没有删掉，删的是指针对应的内存区域上的值
            delete [] tr;
        }
/*
我的rotate和寻常rotate定义不大一样
别的rotate(node)都是针对node当作某个子树的根节点，把node一个儿子转上来，把node转下去
*/
        void rotate(int id)
        {
            int parent=Pa(id);
            int dir=which_one(id);
            
        //每一步操作都要注意，双向关系的维护   

        //先搞定原爷爷(左旋右旋都如此)
            tr[Pa(parent)].child[which_one(parent)]=id;
            Pa(id)=Pa(parent);

        //再搞定自己的儿子(让原儿子去跟原父亲建立bilateral relationship)

            //要利用好dir
            //dir=0:id是左儿子，需要顺时针右旋
            //dir=1:id是右儿子。需要逆时针左旋
            tr[parent].child[dir]=tr[id].child[dir^1];
            Pa(tr[id].child[dir^1])=parent;
            
        //最后搞定自己的父亲（让原父亲和自己建立bilateral relationship)
            Pa(parent)=id;
            tr[id].child[dir^1]=parent;

            push_up(parent);
            push_up(id);
        }
        
    
};

#endif

//注意额外换行符

/*
待查询：为何new的要赋值初始化

oo不然会赋随机值
*/

/*
1. 关于 root=0 与 node_tot=0 初始化是否必须
这两个初始化都是必要的：

root 表示树的根节点索引，初始化为 0 表示初始时没有根节点（或者根节点是索引为0的节点，取决于具体实现）
node_tot 表示当前分配的节点总数，初始化为 0 表示还没有分配任何节点
如果不初始化这两个变量，它们将包含随机的垃圾值，导致程序行为不可预测。

2. 关于析构函数中删除 tr 和中括号的含义
在析构函数中看到的 delete [] tr; 是用来释放之前通过 new Tree_Node[n] 分配的动态数组内存的。

new Tree_Node[n] 在堆上分配了一个可以容纳 n 个 Tree_Node 对象的连续内存块，并返回指向第一个元素的指针
delete [] tr 是删除整个数组的正确方法，它会释放之前分配的整块内存
如果只写 delete tr（没有中括号），只会删除数组的第一个元素，而不会释放整个数组，导致内存泄漏
这是 C++ 中处理动态分配数组内存的标准做法。

3. 如何删除指针对应的内容 vs 删除指针本身
在 C++ 中：

当你使用 new 创建对象时，会在堆上分配内存并返回指向该内存的指针
delete pointer 或 delete[] pointer 删除的是指针所指向的内存内容（即释放内存）
指针变量本身仍然存在，但它变成了悬空指针（不再指向有效内存）
如果想将指针设为无效状态，通常将其设置为 nullptr

*/