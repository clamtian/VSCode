/*
 * @lc app=leetcode.cn id=516 lang=cpp
 *
 * [516] 最长回文子序列
 */

// @lc code=start
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        string t = s;
        reverse(t.begin(), t.end());
        int n = s.size();
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
        for(int i = 1; i <= n; ++i){
            for(int j = 1; j <= n; ++j){
                if(s[i - 1] == t[j - 1]){
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }else{
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[n][n];
    }
};
// @lc code=end

