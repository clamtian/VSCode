#include <vector>
#include <string>
#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**************************************************************/
/*单链表*/
const int N = 100010;

// head存储链表头，e[]存储节点的值，ne[]存储节点的next指针，idx表示当前用到了哪个节点
int head, e[N], ne[N], idx;

// 初始化
void init()
{
    head = -1;
    idx = 0;
}

// 在链表头插入一个数a
void insert(int a)
{
    e[idx] = a, ne[idx] = head, head = idx ++ ;
}

// 将头结点删除，需要保证头结点存在
void remove()
{
    head = ne[head];
}



/**************************************************************/
/*双链表*/

const int N = 1e5 + 10;

int m;
int e[N], l[N], r[N];
int idx;

//! 初始化
void init()
{
    l[1] = 0, r[0] = 1;//* 初始化 第一个点的右边是 1   第二个点的左边是 0
    idx = 2;//! idx 此时已经用掉两个点了
}

//* 在第 K 个点右边插入一个 X 
void add(int k, int x)
{
    e[idx] = x;
    l[idx] = k;
    r[idx] = r[k]; //todo 这边的 k 不加 1 ， 输入的时候 k+1 就好
    l[r[k]] = idx;
    r[k] = idx;
    idx++;
}//! 当然在 K 的左边插入一个数 可以再写一个 ， 也可以直接调用我们这个函数，在 k 的左边插入一个 数 等价于在 l[k] 的右边插入一个数 add(l[k],x)

//*删除第 k个 点
void remove(int k)
{
    r[l[k]] = r[k];
    l[r[k]] = l[k];
}

/**************************************************************/
/*栈*/
// tt表示栈顶
int stk[N], tt = 0;

// 向栈顶插入一个数
stk[++tt] = x;

// 从栈顶弹出一个数
tt -- ;

// 栈顶的值
stk[tt];

// 判断栈是否为空
if (tt > 0)
{

}


/**************************************************************/
/*队列*/
// hh 表示队头，tt表示队尾
int q[N], hh = 0, tt = -1;

// 向队尾插入一个数
q[ ++ tt] = x;

// 从队头弹出一个数
hh ++ ;

// 队头的值
q[hh];

// 判断队列是否为空
if (hh <= tt)
{

}


/**************************************************************/
/*双向队列*/

// hh 表示队头，tt表示队尾的后一个位置
int q[N], hh = 0, tt = 0;

// 向队尾插入一个数
q[tt ++ ] = x;
if (tt == N) tt = 0;

// 从队头弹出一个数
hh ++ ;
if (hh == N) hh = 0;

// 队头的值
q[hh];

// 判断队列是否为空
if (hh != tt)
{

}


int main(){
    
    
    return 1;
}