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


// KMP算法 ：求出模板串 P 在模式串 S 中所有出现的位置的起始下标  
// 核心：求前缀函数 https://oi-wiki.org/string/kmp/
vector<int> KMP(string& s, string& p){
    int m = p.size(), n = s.size();
    vector<int> pi(m);
    for(int i = 1, j = 0; i < m; ++i){
        while(j > 0 && p[j] != p[i]) j = pi[j - 1];
        if(p[j] == p[i]) ++j;
        pi[i] = j;
    }
    vector<int> res;
    for(int i = 0, j = 0; i < n; ++i){
        while(j > 0 && p[j] != s[i]) j = pi[j - 1];
        if(p[j] == s[i]) ++j;
        if(j == m){
            res.push_back(i - m + 1);
            j = pi[j - 1];
        }
    }
    return res;
}

// Z函数(拓展KMP)：https://oi-wiki.org/string/z-func/

vector<int> ZTransform(string& s) {
    int n = s.size();
    vector<int> z(n);
    for(int i = 1, l = 0, r = 0; i < n; ++i){
        if (i <= r && z[i - l] < r - i + 1) {
            z[i] = z[i - l];
        } else {
            z[i] = max(0, r - i + 1);
            while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        }
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z; 
}

int main(){
    string s = "babab", p = "babab";
    KMP(s, p);
    return 1;
}