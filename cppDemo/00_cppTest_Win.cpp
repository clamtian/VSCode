#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int N = 100010;
int son[N][26], cnt[N], idx = 0;

void insert(string& s){
    int p = 0, n = s.size();
    for(int i = 0; i < n; ++i){
        int u = s[p] - 'a';
        if(!son[p][u]) son[p][u] = ++idx;
        p = son[p][u];
    }
    cnt[p]++;
}

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