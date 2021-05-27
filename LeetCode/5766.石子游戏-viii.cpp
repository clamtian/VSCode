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
        vector<int> dp(n + 1, 0);
        dp[n - 1] = stones[n - 1];
        for(int i = n - 2; i >= 0; --i){
            dp[i] = dp[i + 1] + stones[i];
        }
        int pos = 1, sumA = stones[0], sumB = 0;
        while(pos < n - 1){
            int target = dp[pos + 1], index = pos + 1;
            for(int i = pos + 1; i < n; ++i){
                if(target >= dp[i]){
                    index = i;
                    target = dp[index];
                }
            }
            sumA += dp[pos] - dp[index];
            if(index < n) sumB += dp[pos] - dp[index];
            pos = index;
            target = dp[pos + 1], index = pos + 1;
            for(int i = pos + 1; i < n; ++i){
                if(target <= dp[i]){
                    index = i;
                    target = dp[index];
                }
            }
            sumB += dp[pos] - dp[index];
            if(index < n) sumA += dp[pos] - dp[index];
            pos = index;
        }
        return sumA - sumB;
    }
};
// @lc code=end

