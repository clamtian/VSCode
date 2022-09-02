#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;


//***********************************************************************
//拓扑排序O(n + m) n个点m条边
/*
若一个由图中所有点构成的序列 A 满足：对于图中的每条边 (x,y)，x 在 A 中都出现在 y 之前，则称 A 是该图的一个拓扑序列
*/

// A 的格式为 [a, b]， 表示有一条 a->b 的边
vector<int> topSort(int n, vector<vector<int>>& A) {
    vector<int> degree(n);
    vector<vector<int>> edges(n);
    vector<int> res;
    for (auto& v : A) {
        degree[v[1]]++;
        edges[v[0]].push_back(v[1]);
    }
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 0) {
            q.push(i);
            res.push_back(i);
        }
    }
    while (q.size()) {
        int k = q.front();
        q.pop();
        for (int v : edges[k]) {
            --degree[v];
            if (degree[v] == 0) {
                q.push(v);
                res.push_back(v);
            }
        }
    }
    return res;
    
}

int main() {

    int n, m;
    cin >> n >> m;
    vector<vector<int>> edges;
    while (m) {
        --m;
        int a, b;
        cin >> a >> b;
        edges.push_back({ a - 1, b - 1 });
    }
    vector<int> order = topSort(n, edges);

    // 如果所有点都入过队了，说明存在拓扑序列；否则不存在拓扑序列

    if (order.size() == n) {
        for (int t : order) cout << t + 1 << " ";
    } else cout << -1;
    return 0;
}
