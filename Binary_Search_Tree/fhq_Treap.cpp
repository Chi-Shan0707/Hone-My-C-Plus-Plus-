#include<bits/stdc++.h>
using namespace std;
const int N=100010;
const int INF=1000000004;
int n,tot,root;
inline int input()
{
	int num;
	scanf("%d",&num);
	return num;
}
struct Treap
{
	int v,w;//v:Ȩֵ w:���������ֵ 
	int sz;//������С
//���ﲻ�ϲ�Ȩֵ��ͬ�Ľڵ�
	int l,r;//�������������±� 
}tr[N];
inline void new_node(int num)
{
	++tot;
	tr[tot].v=num;tr[tot].w=rand()%123456789;
	tr[tot].sz=1;
	tr[tot].l=tr[tot].r=0;	
}
inline void update(int cur){tr[cur].sz=tr[tr[cur].l].sz+tr[tr[cur].r].sz+1;}
//�����ڵ���Ϣ���ϴ���
void split(int cur,int key,int &x,int &y)//���սڵ�Ȩֵkey���� 
/*
cur�ǵ�ǰ�ڵ�,key�Ƿֵı�׼��x������ǿ÷ֳ���������
һ������ڵ��λ�ñ��
y�����ұ��ǿ÷ֳ�����������һ���ܼӵĽڵ���
*/ 
{
	if(!cur)x=y=0;//���ʵ�Ҷ�ӽڵ�,����0
	else
	{
//��cur���ֳ�Ȩֵ��key�ĺ�Ȩֵ>key�� 
		if(tr[cur].v<=key)
	//cur����ȥ�����,����������Ҳ������ȥ����� 
		{
			x=cur;
			split(tr[cur].r,key,tr[cur].r,y); 
		}
		else
		{
	//cur����ȥ�ұ���,����������Ҳ������ȥ�ұ���
			y=cur;
			split(tr[cur].l,key,x,tr[cur].l);
		}
		update(cur);
		//������£���ָ��cur���ֵ��������ж����Ǹ��»��ֵĵ���и��� 
	}
}
int merge(int x,int y)
//��ǰҪ�ϲ���������������ߵĽڵ�����x�ұߵĽڵ�����y 
{
	if(!x||!y)return x+y;
/*
if(!x)return y;
if(!x)return x;
������Է��ʵ�Ҷ�ӽڵ�0û���ٺϲ���ȥ�˵���� 
*/
	if(tr[x].w<tr[y].w)
//��x�ᵽ����ȥ����x����������y�ϲ�ȥ,x���������Ѿ�ok�� 
	{
		tr[x].r=merge(tr[x].r,y);
		update(x);
//����Ƿ���x��y����˭�������ĸ���������x 
		return x;
	}
	else
	//��y�ᵽ����ȥ����x��y���������ϲ�ȥ,y���������Ѿ�ok�� 
	{
		tr[y].l=merge(x,tr[y].l);
		update(y);
//����Ƿ���x��y����˭�������ĸ���������y 
		return y;
	}
}
inline void insert(int &root,int num)
{
	new_node(num);//����һ���ڵ�Ĳ��� 
	int x,y;
	split(root,num,x,y);
//���ѳ��������ټ���numһ��ϲ� 
	root=merge(merge(x,tot),y);
}
inline void del(int &root,int num)
{
	int x,y,cnt;
	//�ڵ�vֵȫ��num�����ĸ�Ϊcnt
	split(root,num,x,y);
	split(x,num-1,x,cnt);
/*
ע�������д��,x��ָ���ѿ�ʼ�ĵ�;
x,cnt��ʾ�����ֳ����������ĸ�
split(cur,key,&x,&y)
&x�ĸ��Ĳ���Ӱ��cur 
*/
	cnt=merge(tr[cnt].l,tr[cnt].r);
/*
����ĺϲ�������tr[cnt]��ֱ�ӽ����������Ӳ���һ��
����,�Ѿ�del(num)�� 
*/
	root=merge(merge(x,cnt),y);
}
inline int query_rank(int root,int num)
{
	int x,y,res;
	split(root,num-1,x,y);
	res=tr[x].sz+1;
	root=merge(x,y);
	return res;
}
inline int query_num(int cur,int q)
{
	/*
	if(tr[tr[cur].l].sz>=q)return query_num(tr[cur].l,q);
	else if(tr[tr[cur].l].sz+1<q)return query_num(tr[cur].r,q-tr[cur].l-1);
	return tr[cur].v;
	*/
	while(cur)
	{
		if(q<=tr[tr[cur].l].sz)cur=tr[cur].l;
		else if(tr[tr[cur].l].sz+1<q)
		{
			q-=(tr[tr[cur].l].sz+1);
			cur=tr[cur].r;
		}
		else return tr[cur].v;
	}
	return -1;
}
inline int pre(int root,int num)
{
	int x,y,res=-INF;
	split(root,num-1,x,y);
//x����ͷ��v��С��num����num��ǰ���൱����x�������� 
	int cur;cur=x;
	while(cur)
	{
		res=tr[cur].v;
		cur=tr[cur].r;
	}
	root=merge(x,y);
	return res;
}
inline int succ(int root,int num)
{
	int x,y,res=INF;
	split(root,num,x,y);
//y����ͷ��v������num����num�ĺ���൱����y������С��
	int cur;cur=y;
	while(cur)
	{
		res=tr[cur].v;
		cur=tr[cur].l;
	}
	root=merge(x,y);
	return res;
}
int main()
{
	srand(time(0));
	n=input();
	for(int k=1;k<=n;++k)
	{
		int op,m;op=input();m=input();
        if(op==1)insert(root,m);
        else if(op==2)del(root,m);
        else if(op==3)printf("%d\n",query_rank(root,m));
        else if(op==4)printf("%d\n",query_num(root,m));
		else if(op==5)printf("%d\n",pre(root,m));
        else printf("%d\n",succ(root,m));
    }
	return 0;
}
