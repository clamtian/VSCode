// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <bitset>
#include "commoncppproblem600.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=600 lang=cpp
 *
 * [600] 不含连续1的非负整数
 */

// @lc code=start
class Solution {
public:
    int findIntegers(int n) {
        bitset<32> b(n);
        vector<vector<int>> dp(32, vector<int>(2, 0));
        
        int k = 31;
        while(k >= 0){
            if(b[k] == 1) break;
            --k;
        }
        int highBit = k;
        dp[k][0] = dp[k][1] = 1;
        for(k -= 1; k >= 0; --k){
            dp[k][0] = dp[k + 1][0] + dp[k + 1][1];
            dp[k][1] = dp[k + 1][0];
        }
        int ans = 1;
        for(k = 0; k <= highBit; ++k){
            if(b[highBit - k] == 1){
                ans += dp[k][0];
            }
            if(b[highBit - k] == 1 && b[highBit - k + 1] == 1){
                ans -= 1;
                break;
            }
        }
        return ans;
    }
};
// @lc code=end

