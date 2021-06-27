/*
 * @lc app=leetcode.cn id=329 lang=cpp
 *
 * [329] 矩阵中的最长递增路径
 */

// @lc code=start
class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0));
        int ans = 0;
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                ans = max(ans, dfs(matrix, dp, i, j));
            }
        }
        return ans;
    }
    int dfs(vector<vector<int>>& mt, vector<vector<int>>& dp, int r, int c){
        if(dp[r][c] != 0) return dp[r][c];
        dp[r][c] = 1;
        int m = mt.size(), n = mt[0].size();
        if(r - 1 >= 0 && mt[r - 1][c] < mt[r][c]){
            dp[r][c] = max(dp[r][c], dfs(mt, dp, r - 1, c) + 1);
        }
        if(r + 1 < m && mt[r + 1][c] < mt[r][c]){
            dp[r][c] = max(dp[r][c], dfs(mt, dp, r + 1, c) + 1);
        }
        if(c - 1 >= 0 && mt[r][c - 1] < mt[r][c]){
            dp[r][c] = max(dp[r][c], dfs(mt, dp, r, c - 1) + 1);
        }
        if(c + 1 < n && mt[r][c + 1] < mt[r][c]){
            dp[r][c] = max(dp[r][c], dfs(mt, dp, r, c + 1) + 1);
        }
        return dp[r][c];
    }
};
// @lc code=end

