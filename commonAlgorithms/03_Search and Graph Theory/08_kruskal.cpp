#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

/*
给定一个 n 个点 m 条边的无向图，图中可能存在重边和自环，边权可能为负数。

求最小生成树的树边权重之和，如果最小生成树不存在则输出 impossible。

*/
const int N = 100010;
vector<vector<int>> edges;
int p[N];
int n, m;

int find(int x){
    if(x != p[x]) p[x] = find(p[x]);
    return p[x];
}

int kruskal(){
    int ans = 0, cnt = 0;
    sort(edges.begin(), edges.end());
    for(int i = 1; i <= n; ++i) p[i] = i;
    for(auto edge : edges){
        int a = find(edge[1]), b = find(edge[2]);
        if(a != b){
            p[a] = b;
            ans += edge[0];
            ++cnt;
        }
    }
    if(cnt < n - 1) return 0x3f3f3f3f;
    return ans;
}
int main(){
    cin >> n >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0, w = 0;
        cin >> a >> b >> w;
        edges.push_back({ w, a, b });
    }
    
    int ans = kruskal();
    if(ans < 0x3f3f3f3f) cout << ans;
    else cout << "impossible";
    return 0;
}