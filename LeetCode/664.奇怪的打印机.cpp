/*
 * @lc app=leetcode.cn id=664 lang=cpp
 *
 * [664] 奇怪的打印机
 */

// @lc code=start
class Solution {
public:
    int strangePrinter(string s) {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
        for(int i = 0; i < n; ++i){
            dp[i][i] = 1;
        }
        for(int j = 1; j < n; ++j){
            for(int i = 0; i + j < n; ++i){
                if(s[i] == s[i + j]){
                    dp[i][i + j] = dp[i + 1][i + j];
                }else{
                    for(int k = i; k < i + j; ++k){
                        dp[i][i + j] = min(dp[i][i + j], dp[i][k] + dp[k + 1][i + j]);
                    }
                }
            }
        }
        return dp[0][n - 1];
    }
};
// @lc code=end

