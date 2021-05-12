/*
 * @lc app=leetcode.cn id=552 lang=cpp
 *
 * [552] 学生出勤记录 II
 */

// @lc code=start
class Solution {
public:
    int checkRecord(int n) {
        int MOD = 1e9 + 7;
        vector<vector<long long>> dp(n, vector<long long>(6, 0));
        dp[0][0] = dp[0][3] = dp[0][5] = 1;
        for(int i = 1; i < n; ++i){
            dp[i][0] = dp[i - 1][0] + dp[i - 1][1] + dp[i - 1][2] + dp[i - 1][3] + dp[i - 1][4] + dp[i - 1][5];
            dp[i][1] = dp[i - 1][0];
            dp[i][2] = dp[i - 1][1];
            dp[i][3] = dp[i - 1][5];
            dp[i][4] = dp[i - 1][3];
            dp[i][5] = dp[i - 1][3] + dp[i - 1][4] + dp[i - 1][5];
            if(dp[i][0] > MOD) dp[i][0] %= MOD;
            if(dp[i][5] > MOD) dp[i][5] %= MOD;
        }
        long long ans = dp[n - 1][0] + dp[n - 1][1] + dp[n - 1][2] + dp[n - 1][3] + dp[n - 1][4] + dp[n - 1][5];
        return ans % MOD;
    }
};
// @lc code=end

