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
const int N = 100010;
// cnt[i] 存储点i的入度
int h[N], e[2 * N], ne[2 * N], st[2 * N], cnt[N], idx;
int n, m;
queue<int> q;
vector<int> order;

void add(int a, int b){
    e[idx] = b;
    ne[idx] = h[a];
    h[a] = idx++;
    ++cnt[b];
}

void topSort(){
    for(int i = 1; i <= n; ++i){
        if(!cnt[i]){
            q.push(i);
            order.push_back(i);
        } 
    }
    while(!q.empty()){
        int a = q.front();
        q.pop();
        for(int i = h[a]; i != -1; i = ne[i]){
            int j = e[i];
            --cnt[j];
            if(cnt[j] == 0){
                q.push(j);
                order.push_back(j);
            } 
        }
    }
}

int main(){
    
    cin >> n >> m;
    idx = 0;
    memset(h, -1, sizeof(h));
    while(m--){
        int a = 0, b = 0;
        cin >> a >> b;
        add(a, b);
    }
    
    topSort();
    // 如果所有点都入过队了，说明存在拓扑序列；否则不存在拓扑序列
    if(order.size() == n){
        for(int i = 0; i < n; ++i) cout << order[i] << " ";
    }else{
        cout << -1;
    } 
    
    return 0;
}