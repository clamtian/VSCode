/*
 * @lc app=leetcode.cn id=1269 lang=cpp
 *
 * [1269] 停在原地的方案数
 */

// @lc code=start
class Solution {
public:
    int numWays(int steps, int arrLen) {
        int MOD = 1e9 + 7;
        vector<long long> dp1(arrLen + 2, 0), dp2(arrLen + 2, 0);
        dp1[1] = 1;
        for(int i = 1; i <= steps; ++i){
            for(int j = 1; j <= (arrLen > steps - i ? steps - i + 1 : arrLen); ++j){
                dp2[j] = dp1[j - 1] + dp1[j + 1] + dp1[j];
                if(dp2[j] > MOD) dp2[j] %= MOD;
            }
            swap(dp1, dp2);
        }
        return dp1[1] % MOD;
    }
    
};
// @lc code=end

