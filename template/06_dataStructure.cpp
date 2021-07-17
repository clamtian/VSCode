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


const int N = 100010;
int idx, head, e[N], ne[N];

//添加头结点
void add_head(int x){
    e[idx] = x;
    ne[idx] = head;
    head = idx++;
}
//在第k个插入的节点后添加
void add(int k, int x){
    e[idx] = x;
    ne[idx] = ne[k];
    ne[k] = idx++;
}
//删除第k个插入的节点
void remove(int k){
    ne[k] = ne[ne[k]];
}




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









int main(){
    
    
    return 1;
}