/*
 * @lc app=leetcode.cn id=5766 lang=cpp
 *
 * [5766] 石子游戏 VIII
 */

// @lc code=start
class Solution {
public:
    int stoneGameVIII(vector<int>& stones) {
        int n = stones.size();
        vector<int> sum(n + 1, 0);
        for(int i = 0; i < n; ++i){
            sum[i + 1] = sum[i] + stones[i];
        }
        int ans = sum[n];
        for(int i = n - 1; i > 1; --i){
            ans = max(ans, sum[i] - ans);
        }
        return ans;
    }
};
// @lc code=end

