// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include "commoncppproblem752.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=752 lang=cpp
 *
 * [752] 打开转盘锁
 */

// @lc code=start
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> uset(deadends.begin(), deadends.end());
        vector<int> dp1(10000, -2), dp2(10000, -2);
        dp1[0] = dp2[0] = 0;
        int cmp1 = dfs1(uset, target, dp1);
        cout << cmp1 << endl;
        int cmp2 = dfs2(uset, target, dp2, dp1);
        cout << cmp2 << endl;
        int ans = min(cmp1, cmp2);
        return ans == -1 ? max(cmp1, cmp2) : ans;
    }
    int dfs2(unordered_set<string>& uset, string t, vector<int>& dp, vector<int>& dp1){
        if(dp[stoi(t)] == -2){
            if(uset.find(t) != uset.end()){
                dp[stoi(t)] = -1;
            }else{
                string s;
                int cmp, tmp = INT_MAX;

                s = t;
                if(t[0] != '0'){
                    s[0] = t[0] - 1;
                    cmp = dfs2(uset, s, dp, dp1);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }
                
                s = t;
                if(t[1] != '0'){
                    s[1] = t[1] - 1;
                    cmp = dfs2(uset, s, dp, dp1);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                s = t;
                if(t[2] != '0'){
                    s[2] = t[2] - 1;
                    cmp = dfs2(uset, s, dp, dp1);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                s = t;
                if(t[3] != '0'){
                    s[3] = t[3] - 1;
                    cmp = dfs2(uset, s, dp, dp1);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }
                if(tmp == INT_MAX) dp[stoi(t)] = -1;
                else dp[stoi(t)] = tmp;
            }
        } 
        if(dp1[stoi(t)] != -1) 
            dp[stoi(t)] = dp[stoi(t)] == -1 ? dp1[stoi(t)] : min(dp[stoi(t)], dp1[stoi(t)]);
        return dp[stoi(t)];
    }
    int dfs1(unordered_set<string>& uset, string t, vector<int>& dp){
        if(dp[stoi(t)] == -2){
            if(uset.find(t) != uset.end()){
                dp[stoi(t)] = -1;
            }else{
                string s;
                int cmp, tmp = INT_MAX;

                s = t;
                if(t[0] != '9'){
                    s[0] = t[0] + 1;
                    cmp = dfs1(uset, s, dp);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                s = t;
                if(t[1] != '9'){
                    s[1] = t[1] + 1;
                    cmp = dfs1(uset, s, dp);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                s = t;
                if(t[2] != '9'){
                    s[2] = t[2] + 1;
                    cmp = dfs1(uset, s, dp);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                s = t;
                if(t[3] != '9'){
                    s[3] = t[3] + 1;
                    cmp = dfs1(uset, s, dp);
                    if(cmp != -1) tmp = min(tmp, cmp + 1);
                }

                if(tmp == INT_MAX) dp[stoi(t)] = -1;
                else dp[stoi(t)] = tmp;
            }
            
        } 
        return dp[stoi(t)];
    }
};
// @lc code=end

