/*
 * @lc app=leetcode.cn id=363 lang=cpp
 *
 * [363] 矩形区域不超过 K 的最大数值和
 */

// @lc code=start
class Solution {
public:
    int maxSumSubmatrix(vector<vector<int>>& matrix, int k) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> dp(n + 1, vector<int>(m, 0));
        for(int i = 1; i < n + 1; ++i){
            for(int j = 0; j < m; ++j){
                dp[i][j] = dp[i - 1][j] + matrix[j][i - 1];
            }
        }
        int ans = INT_MIN;
        for(int i = 1; i <= n; ++i){
            for(int j = 0; j < i; ++j){
                vector<int> vec(m, 0);
                for(int p = 0; p < m; ++p){
                    vec[p] = dp[i][p] - dp[j][p];
                }
                ans = max(ans, findK(vec, k));
            }
        }
        return ans;
    }
    int findK(vector<int>& vec, int k){
        int ans = vec[0], dp = vec[0];
        int n = vec.size();
        for(int i = 1; i < n; ++i){
            if(dp <= 0){
                dp = vec[i]; 
            }else{
                dp += vec[i];
            }
            ans = max(ans, dp);
        }
        if(ans <= k) return ans;
        ans = INT_MIN;
        vector<int> sum(n + 1, 0);
        for(int i = 1; i <= n; ++i){
            sum[i] = sum[i - 1] + vec[i - 1];
        }
        for(int i = 1; i <= n; ++i){
            for(int j = 0; j < i; ++j){
                int cmp = sum[i] - sum[j];
                if(cmp <= k) ans = max(cmp, ans);
            }
        }
        return ans;
    }
};
// @lc code=end

