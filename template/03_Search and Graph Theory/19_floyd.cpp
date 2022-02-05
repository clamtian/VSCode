#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;

/*
给定一个 n 个点 m 条边的有向图，图中可能存在重边和自环，边权可能为负数。

再给定 k 个询问，每个询问包含两个整数 x 和 y，表示查询从点 x 到点 y 的最短距离，如果路径不存在，则输出 impossible。

数据保证图中不存在负权回路。
*/
/*
f[i, j, k]表示从i走到j的路径上除i和j点外只经过1到k的点的所有路径的最短距离。那么f[i, j, k] = min(f[i, j, k - 1), f[i, k, k - 1] + f[k, j, k - 1]。
因此在计算第k层的f[i, j]的时候必须先将第k - 1层的所有状态计算出来，所以需要把k放在最外层。
*/
const int N = 205;
int d[N][N];
int n, m, k;

void floyd(){
    for(int k = 1; k <= n; ++k){
        for(int i = 1; i <= n; ++i){
            for(int j = 1; j <= n; ++j){
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
}

int main(){
    memset(d, 0x3f, sizeof(d));
    cin >> n >> m >> k;
    int a = 0, b = 0, w = 0;
    for(int i = 1; i <= n; ++i) d[i][i] = 0;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0, w = 0;
        cin >> a >> b >> w;
        d[a][b] = min(d[a][b], w);
    }
    
    floyd();
    while(k--){
        cin >> a >> b;
        if(d[a][b] < 0x3f3f3f3f / 2) cout << d[a][b] << endl;
        else cout << "impossible" << endl;
    }
    return 0;
}