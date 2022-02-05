#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

//***********************************************************************
//带负权边的Bellman_Ford算法

/*
给定一个 n 个点 m 条边的有向图，图中可能存在重边和自环， 边权可能为负数。

请你求出从 1 号点到 n 号点的最多经过 k 条边的最短距离，如果无法从 1 号点走到 n 号点，输出 impossible。

注意：图中可能 存在负权回路 。

*/

const int N = 510;
int dist[N], back[N];
vector<vector<int>> edge;
int n, m, k;

void bellman_ford(){
    memset(dist, 0x3f, sizeof(dist));
    dist[1] = 0;
    
    for(int i = 1; i <= k; ++i){
        memcpy(back, dist, sizeof(dist));
        for(int j = 0; j < m; ++j){
            int a = edge[j][0], b = edge[j][1], w = edge[j][2];
            dist[b] = min(dist[b], back[a] + w);
        }
        
    }
}
int main(){
    
    return 0;
}