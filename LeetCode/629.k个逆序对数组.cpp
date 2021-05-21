// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem629.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=629 lang=cpp
 *
 * [629] K个逆序对数组
 */

// @lc code=start
class Solution {
public:
    int kInversePairs(int n, int k) {
        if(k == 0) return 1;
        int MOD = 1e9 + 7;
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, 0));
        for(int i = 0; i <= n; ++i){
            dp[i][0] = 1;
        }
        for(int i = 1; i <= n; ++i){
            for(int j = 1; j <= k; ++j){
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                if(j >= i) dp[i][j] += MOD - dp[i - 1][j - i];
                if(dp[i][j] >= MOD) dp[i][j] %= MOD;
            }
        }
        return dp[n][k];
    }
};
// @lc code=end

