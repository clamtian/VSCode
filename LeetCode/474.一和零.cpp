/*
 * @lc app=leetcode.cn id=474 lang=cpp
 *
 * [474] 一和零
 */

// @lc code=start
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for(int i = 0; i < len; ++i){
            int num0 = 0, num1 = 0;
            for(char c : strs[i]){
                if(c == '0'){
                    ++num0;
                }else{
                    ++num1;
                }
            }
            for(int i = m; i >= num0; --i){
                for(int j = n; j >= num1; --j){
                    dp[i][j] = max(dp[i][j], dp[i - num0][j - num1] + 1);
                }
            }
        }
        
        return dp[m][n];
    }
};
// @lc code=end

