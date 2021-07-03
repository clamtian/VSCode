// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <numeric>
#include "commoncppproblem416.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=416 lang=cpp
 *
 * [416] 分割等和子集
 */

// @lc code=start
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        int n = nums.size();
        if(sum % 2 != 0 || n < 2) return false;
        int target = sum / 2;
        vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
        for(int i = 0; i <= n; ++i) dp[i][0] = 1;
        for(int i = 1; i <= n; ++i){
            
            for(int j = target; j >= nums[i - 1]; --j){
                dp[i][j] = dp[i][j] || dp[i][j - nums[i - 1]];
            }
            if(nums[i - 1] <= target) dp[i][nums[i - 1]] = 1;
        }
        return dp[n][target];
    }
};
// @lc code=end

