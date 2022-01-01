// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem886.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=886 lang=cpp
 *
 * [886] 可能的二分法
 */

// @lc code=start
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& d) {
        if(d.size() < 2) return true;
        int m = d.size();
        vector<int> p(n + 1, 0), used(m, 0);
        for(int i = 1; i <= n; ++i) p[i] = i;
        return dfs(d, p, d[0][0], used);
    }
    bool dfs(vector<vector<int>>& d, vector<int>& p, int t, vector<int>& used){
        int m = d.size();
        for(int i = 0; i < m; ++i){
            if(used[i]) continue;
            if(d[i][0] == t || d[i][1] == t){
                used[i] = 1;
                int s = d[i][0] == t ? d[i][1] : d[i][0];
                if(p[t] == t){
                    if(p[s] == s){
                        p[t] = 0, p[s] = -1;
                        if(!dfs(d, p, s, used)) return false;
                    }else{
                        p[t] = p[s] == 0 ? -1 : 0;
                    }
                }else{
                    if(p[s] == s){
                        p[s] = p[t] == 0 ? -1 : 0;
                        if(!dfs(d, p, s, used)) return false;
                    }else if(p[s] == p[t]) return false;
                }
            }
            
        }
        return true;
    }
};
// @lc code=end

