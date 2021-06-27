/*
 * @lc app=leetcode.cn id=44 lang=cpp
 *
 * [44] 通配符匹配
 */

// @lc code=start
class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.size(), n = p.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        dp[0][0] = 1;
        for(int j = 0; j < n && p[j] == '*' ; ++j){
            dp[0][j + 1] = 1;
        }
        for(int i = 1; i <= m; ++i){
            for(int j = 1; j <= n; ++j){
                if(s[i - 1] == p[j - 1] || p[j - 1] == '?'){
                    dp[i][j] = dp[i - 1][j - 1];
                }else if(p[j - 1] == '*'){
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
                }
            }
        }
        return dp[m][n];
    }
};
// @lc code=end

