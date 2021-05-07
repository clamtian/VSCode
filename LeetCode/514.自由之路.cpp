// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem514.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=514 lang=cpp
 *
 * [514] 自由之路
 */

// @lc code=start
class Solution {
public:
    int findRotateSteps(string ring, string key) {
        unordered_map<char, vector<int>> m;
        int n = ring.size();
        for(int i = 0; i < ring.size(); ++i){
            m[ring[i]].push_back(i);
        }
        vector<vector<int>> dp;
        for(int i = 0; i < key.size(); ++i){
            dp.push_back(m[key[i]]);
        }
        vector<vector<int>> re = dp;
        for(int i = 0; i < re.back().size(); ++i){
            re.back()[i] = 0;
        }
        for(int i = key.size() - 2; i >= 0; --i){
            for(int j = 0; j < dp[i].size(); ++j){
                int minNum = INT_MAX;
                for(int k = 0; k < dp[i + 1].size(); ++k){
                    int dis1 = abs(dp[i][j] - dp[i + 1][k]);
                    int dis2 = n - abs(dp[i][j] - dp[i + 1][k]);
                    int cmp = re[i + 1][k] + min(dis1, dis2);
                    minNum = min(minNum, cmp);
                }
                re[i][j] = minNum;
            }
        }
        int ans = INT_MAX;
        for(int i = 0; i < re[0].size(); ++i){
            ans = min(ans, re[0][i] + min(dp[0][i], n - dp[0][i]));
        }
        return ans + key.size();
    }
};
// @lc code=end

