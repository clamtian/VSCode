#include <vector>
#include <string>
#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int N = 100010;
int son[N][26], cnt[N], idx = 0;

// 0号点既是根节点，又是空节点
// son[][]存储树中每个节点的子节点
// cnt[]存储以每个节点结尾的单词数量


// 插入一个字符串
void insert(string& s){
    int p = 0, n = s.size();
    for(int i = 0; i < n; ++i){
        int u = s[i] - 'a';
        if(!son[p][u]) son[p][u] = ++idx;
        p = son[p][u];
    }
    cnt[p]++;
}


// 查询字符串出现的次数
int search(string& s){
    int p = 0, n = s.size();
    for(int i = 0; i < n; ++i){
        int u = s[i] - 'a';
        if(!son[p][u]) return 0;
        p = son[p][u];
    }
    return cnt[p];
}


int main(){
    
    int n = 0;
    cin >> n;
    string op, str;
    while(n--){
        cin >> op >> str;
        if(op == "I") insert(str);
        else{
            int c = search(str);
            cout << c << endl;
        }
    }
    return 0;
}

