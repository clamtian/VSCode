#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;


/*
给定一个 n 个点 m 条边的有向图，图中可能存在重边和自环， 边权可能为负数。

请你求出 1 号点到 n 号点的最短距离，如果无法从 1 号点走到 n 号点，则输出 impossible。

数据保证不存在负权回路
*/

/*
1. Bellman_ford算法可以存在负权回路，是因为其循环的次数是有限制的因此最终不会发生死循环；
但是SPFA算法不可以，由于用了队列来存储，只要发生了更新就会不断的入队，因此假如有负权回路请你不要用SPFA否则会死循环。

2. 由于SPFA算法是由Bellman_ford算法优化而来，在最坏的情况下时间复杂度和它一样即时间复杂度为 O(nm)O(nm) ，
假如题目时间允许可以直接用SPFA算法去解Dijkstra算法的题目。(好像SPFA有点小小万能的感觉?)

3. 求负环一般使用SPFA算法，方法是用一个cnt数组记录每个点到源点的边数，一个点被更新一次就+1，一旦有点的边数达到了n那就证明存在了负环。


*/
const int N = 100010;
int h[N], e[N], ne[N], W[N], dist[N], st[N], idx = 0;
int n, m;

void spfa(){
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    st[1] = 1;
    while(!q.empty()){
        int t = q.front();
        q.pop();
        st[t] = 0;
        for(int i = h[t]; i != -1; i = ne[i]){
            int j = e[i];
            if(dist[j] > dist[t] + W[i]){
                dist[j] = dist[t] + W[i];
                if(!st[j]){
                    q.push(j);
                    st[j] = 1;
                }
            }
        }
    }
}

void add(int a, int b, int w){
    e[idx] = b;
    ne[idx] = h[a];
    W[idx] = w;
    h[a] = idx++;
    
}

int main(){
    memset(dist, 0x3f, sizeof(dist));
    memset(h, -1, sizeof(h));
    memset(st, 0, sizeof(st));
    cin >> n >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0, w = 0;
        cin >> a >> b >> w;
        add(a, b, w);
    }
    
    spfa();
    if(dist[n] < 0x3f3f3f3f) cout << dist[n];
    else cout << "impossible";
    
    return 0;
}


/*
给定一个 n 个点 m 条边的有向图，图中可能存在重边和自环， 边权可能为负数。

请你判断图中是否存在负权回路。
*/


#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;

const int N = 100010;
int h[N], e[N], ne[N], W[N], dist[N], st[N],cnt[N], idx = 0;  // dist[x]存储1号点到x的最短距离，cnt[x]存储1到x的最短路中经过的点数
int n, m;

bool spfa(){
    queue<int> q;

    // 不需要初始化dist数组  有负环的话dist为0也会被更新
    // 原理：如果某条最短路径上有n个点（除了自己），那么加上自己之后一共有n+1个点，由抽屉原理一定有两个点相同，所以存在环。


    //1号点可能没在环上  需要把所有点都当做一号点遍历一次
    for (int i = 1; i <= n; i ++ ){
        q.push(i);
        st[i] = 1;
    }
    while(!q.empty()){
        int t = q.front();
        q.pop();
        st[t] = 0;
        for(int i = h[t]; i != -1; i = ne[i]){
            int j = e[i];
            if(dist[j] > dist[t] + W[i]){
                cnt[j] += 1;
                dist[j] = dist[t] + W[i];
                if(cnt[j] >= n) return true;  // 如果从1号点到x的最短路中包含至少n个点（不包括自己），则说明存在环
                if(!st[j]){
                    q.push(j);
                    st[j] = 1;
                }
            }
        }
    }
    return false;
}

void add(int a, int b, int w){
    e[idx] = b;
    ne[idx] = h[a];
    W[idx] = w;
    h[a] = idx++;
    
}

int main(){
    memset(h, -1, sizeof(h));
    memset(st, 0, sizeof(st));
    cin >> n >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0, w = 0;
        cin >> a >> b >> w;
        add(a, b, w);
    }
    
    bool f = spfa();
    if(f) cout << "Yes";
    else cout << "No";
    return 0;
}