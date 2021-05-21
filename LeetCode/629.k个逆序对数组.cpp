/*
 * @lc app=leetcode.cn id=629 lang=cpp
 *
 * [629] K个逆序对数组
 */

// @lc code=start
class Solution {
public:
    int kInversePairs(int n, int k) {
        if(k == 0) return 1;
        int MOD = 1e9 + 7;
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        for(int i = 2; i <= n; ++i){
            dp[i][0] = 1;
        }
        for(int j = 1; j <= k; ++j){
            for(int i = 1; i <= n; ++i){
                dp[i][j] = dp[i][j - 1] + dp[i - 1][j];
                if(dp[i][j] > MOD) dp[i][j] %= MOD;
            }
        }
        return dp[n][k];
    }
};
// @lc code=end

