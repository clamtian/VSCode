/*
 * @lc app=leetcode.cn id=931 lang=cpp
 *
 * [931] 下降路径最小和
 */

// @lc code=start
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        vector<int> dp1(n, 0), dp2(n, 0);
        dp1 = matrix[0];
        for(int i = 1; i < n; ++i){
            for(int j = 1; j < n - 1; ++j){
                dp2[j] = matrix[i][j] + min(dp1[j - 1], min(dp1[j], dp1[j + 1]));
            }
            dp2[0] = matrix[i][0] + min(dp1[0], dp1[1]);
            dp2[n - 1] = matrix[i][n - 1] + min(dp1[n - 1], dp1[n - 2]);
            swap(dp1, dp2);
        }
        return *min_element(dp1.begin(), dp1.end());
    }
};
// @lc code=end

