/*
 * @lc app=leetcode.cn id=120 lang=cpp
 *
 * [120] 三角形最小路径和
 */

// @lc code=start
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<int> dp(n, 0);
        dp[0] = triangle[0][0];
        for(int i = 1; i < n; ++i){
            dp[i] = triangle[i][i] + dp[i - 1];
            for(int j = i - 1; j > 0; --j){
                dp[j] = triangle[i][j] + min(dp[j], dp[j - 1]);
            }
            dp[0] += triangle[i][0];
        }
        return *min_element(dp.begin(), dp.end());
    }
};
// @lc code=end

