#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

using namespace std;

const int N = 10;
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
            if(cnt[j] == 0) order.push_back(j);
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
    if(order.size() == n){
        for(int i = 0; i < n; ++i) cout << order[i] << " ";
    }else{
        cout << -1;
    } 
    
    return 0;
}