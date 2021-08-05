#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;

/*
给定一个 n 个点 m 条边的无向图，图中可能存在重边和自环。

请你判断这个图是否是二分图。

*/
const int N = 100010;
int h[N], e[2 * N], ne[2 * N], color[N], idx = 0;
int n, m;

bool dfs(int u, int c){
    color[u] = c;
    for(int i = h[u]; i != -1; i = ne[i]){
        int j = e[i];
        if(color[j] == -1){
            if(!dfs(j, !c)) return false;
        }else if(color[j] == c) return false;
    }
    return true;
}
bool check(){
    memset(color, -1, sizeof(color));
    for(int i = 1; i <= n; ++i){
        if(color[i] == -1){
            if(!dfs(i, 0)) return false;
        }
    }
    return true;
}
void add(int a, int b){
    e[idx] = b;
    ne[idx] = h[a]; 
    h[a] = idx++;
}
int main(){
    memset(h, -1, sizeof(h));
    
    cin >> n >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0;
        cin >> a >> b;
        add(a, b), add(b, a);
    }
    
    if(check()) cout << "Yes";
    else cout << "No";
    return 0;
}