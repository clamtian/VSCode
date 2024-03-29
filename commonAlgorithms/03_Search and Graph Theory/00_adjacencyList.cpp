#include <iostream>
#include <string>

using namespace std;


/*
这招适合用来炫技
一般写题时候就不这么搞了
直接vector<vector<int>>或者用map<int, vector<int>>就好了
写这个容易出错
*/

//邻接表存储有向图   头插法
const int N = 100010;

// 对于每个点k，开一个单链表，存储k所有可以走到的点。h[k]存储这个单链表的头结点
int h[N], e[N], ne[N], idx;

// 添加一条边 a->b
void add(int a, int b){
    e[idx] = b, ne[idx] = h[a], h[a] = idx ++ ;
}

// 初始化
idx = 0;
memset(h, -1, sizeof(h));