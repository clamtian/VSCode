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


//求出模板串 P 在模式串 S 中所有出现的位置的起始下标
vector<int> KMP(string& s, string& p){
    int m = p.size(), n = s.size();
    vector<int> dp(m + 1, -1);
    for(int i = 1, j = -1; i < m; ++i){
        while(j >= 0 && p[j + 1] != p[i]) j = dp[j];
        if(p[j + 1] == p[i]) ++j;
        dp[i] = j;
    }
    vector<int> res;
    for(int i = 0, j = -1 ; i < n; ++i){
        while(j >= 0 && p[j + 1] != s[i]) j = dp[j];
        if(p[j + 1] == s[i]) ++j;
        if(j == m - 1){
            res.push_back(i - m + 1);
            j = dp[j];
        }
    }
    return res;
}

int main(){

    return 1;
}