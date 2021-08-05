#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;

/*
给定一个 n 个点 m 条边的无向图，图中可能存在重边和自环，边权可能为负数。

求最小生成树的树边权重之和，如果最小生成树不存在则输出 impossible。
*/

const int N = 505;
int g[N][N], dist[N], st[N], idx = 0;
int n, m;

int prim(){
    dist[1] = 0;
    int ans = 0;
    for(int i = 1; i <= n; ++i){
        int t = -1;
        for(int j = 1; j <= n; ++j){
            if(!st[j] && (t == -1 || dist[t] > dist[j])) 
                t = j;
        }
        if(dist[t] == 0x3f3f3f3f) return 0x3f3f3f3f;
        st[t] = 1;
        ans += dist[t];
        for(int j = 1; j <= n; ++j)
            dist[j] = min(dist[j], g[t][j]);
    }
    return ans;
}
int main(){
    memset(g, 0x3f, sizeof(g));
    memset(st, 0, sizeof(st));
    memset(dist, 0x3f, sizeof(dist));
    cin >> n >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0, w = 0;
        cin >> a >> b >> w;
        g[a][b] = g[b][a] = min(g[a][b], w);
    }
    
    int ans = prim();
    if(ans < 0x3f3f3f3f) cout << ans;
    else cout << "impossible";
    return 0;
}