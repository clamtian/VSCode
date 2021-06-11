// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <map>
#include "commoncppproblem486.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=486 lang=cpp
 *
 * [486] 预测赢家
 */

// @lc code=start
class Solution {
public:
    bool PredictTheWinner(vector<int>& nums) {
        int n = nums.size();
        if(n == 1) return true;
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        for(int i = 0; i < n; ++i){
            dp[i][i + 1] = nums[i];
        }
        for(int j = 1; j <= n; ++j){
            for(int i = j - 2; i >= 0; --i){
                dp[i][j] = max(nums[i] - dp[i + 1][j], nums[j - 1] - dp[i][j - 1]);
            }
        }
        return dp[0][n] >= 0;
    }
};
// @lc code=end

