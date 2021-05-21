/*
 * @lc app=leetcode.cn id=1035 lang=cpp
 *
 * [1035] 不相交的线
 */

// @lc code=start
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size(), ans = 0;
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                if(nums1[i] == nums2[j]){
                    dp[i + 1][j + 1] = max(dp[i][j] + 1, max(dp[i + 1][j], dp[i][j + 1]));
                }else{
                    dp[i + 1][j + 1] = max(dp[i][j], max(dp[i + 1][j], dp[i][j + 1]));
                }
                ans = max(ans, dp[i + 1][j + 1]);
            }
        }
        return ans;
    }
};
// @lc code=end

