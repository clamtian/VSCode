#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

//***********************************************************************
//朴素dijkstra O(n*n + m)
int g[N][N];  // 存储每条边
int dist[N];  // 存储1号点到每个点的最短距离
bool st[N];   // 存储每个点的最短路是否已经确定

// 求1号点到n号点的最短路，如果不存在则返回-1
int dijkstra(){
    memset(dist, 0x3f, sizeof dist);
    dist[1] = 0;

    for (int i = 0; i < n - 1; i ++ ){
        int t = -1;     // 在还未确定最短路的点中，寻找距离最小的点
        for (int j = 1; j <= n; j ++ )
            if (!st[j] && (t == -1 || dist[t] > dist[j]))
                t = j;

        // 用t更新其他点的距离
        for (int j = 1; j <= n; j ++ )
            dist[j] = min(dist[j], dist[t] + g[t][j]);

        st[t] = true;
    }

    if (dist[n] == 0x3f3f3f3f) return -1;
    return dist[n];
}

//***********************************************************************
//堆优化dijkstra O(mlogn)

typedef pair<int, int> PII;
const int N = 150100;
//w寸权重
int h[N], e[N], ne[N], st[N], w[N], dist[N], idx;
int n, m;


void add(int a, int b, int v){
    e[idx] = b;
    w[idx] = v;
    ne[idx] = h[a];
    h[a] = idx++;
}

void dijkstra(){
    for(int i = 1; i <= n; ++i) dist[i] = INT_MAX;
    dist[1] = 0;
    memset(h, -1, sizeof(h));
    memset(st, 0, sizeof(st));
    memset(dist, 0x3f, sizeof(dist));
    
    priority_queue<PII, vector<PII>, greater<PII>> q;
    
    q.push({ 0, 1 });
    while(!q.empty()){
        auto p = q.top();
        q.pop();
        
        int node = p.second, distance = p.first;
        
        if(st[node]) continue;
        st[node] = 1;
        
        for(int i = h[node]; i != -1; i = ne[i]){
            int j = e[i];
            if(dist[j] > dist[node] + w[i]){
                dist[j] = dist[node] + w[i];
                q.push({ dist[j], j });
            }
        }
    }
}

int main(){
     
    return 0;
}