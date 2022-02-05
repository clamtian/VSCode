#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>

using namespace std;

/*
给定一个二分图，其中左半部包含 n1 个点（编号 1∼n1），右半部包含 n2 个点（编号 1∼n2），二分图共包含 m 条边。

数据保证任意一条边的两个端点都不可能在同一部分中。

请你求出二分图的最大匹配数。

*/

const int N = 100010;
int n1, n2;     // n1表示第一个集合中的点数，n2表示第二个集合中的点数
int h[N], e[M], ne[M], idx;     // 邻接表存储所有边，匈牙利算法中只会用到从第一个集合指向第二个集合的边，所以这里只用存一个方向的边
int match[N];       // 存储第二个集合中的每个点当前匹配的第一个集合中的点是哪个
bool st[N];     // 表示第二个集合中的每个点是否已经被遍历过

bool find(int x){
    for(int i = h[x]; i != -1; i = ne[i]){
        int j = e[i];
        if(!st[j]){
            st[j] = 1;
            if(match[j] == 0 || find(match[j])){
                match[j] = x;
                return true;
            }
        }
    }
    return false;
}
void add(int a, int b){
    e[idx] = b;
    ne[idx] = h[a]; 
    h[a] = idx++;
}
int main(){
    memset(h, -1, sizeof(h));
    
    cin >> n1 >> n2 >> m;
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0;
        cin >> a >> b;
        add(a, b);
    }

    // 求最大匹配数，依次枚举第一个集合中的每个点能否匹配第二个集合中的点
    int ans = 0;
    for(int i = 1; i <= n1; ++i){
        memset(st, 0, sizeof(st));  //消除重边影响
        if(find(i)) ++ans;
    }
    cout << ans << endl;
    return 0;
}