#include <iostream>
#include <string>

using namespace std;


//************************************************************************
//DFS

int dfs(int u){
    st[u] = true; // st[u] 表示点u已经被遍历过

    for (int i = h[u]; i != -1; i = ne[i]){
        int j = e[i];
        if (!st[j]) dfs(j);
    }
}

//************************************************************************
//BFS

queue<int> q;
st[1] = true; // 表示1号点已经被遍历过
q.push(1);

while (q.size()){
    int t = q.front();
    q.pop();
    for (int i = h[t]; i != -1; i = ne[i]){
        int j = e[i];
        if (!st[j]){
            st[j] = true; // 表示点j已经被遍历过
            q.push(j);
        }
    }
}
