// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <algorithm>
#include "commoncppproblem377.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=377 lang=cpp
 *
 * [377] 组合总和 Ⅳ
 */

// @lc code=start
class Solution {
public:
    int combinationSum4(vector<int>& nums, int target) {
        nums.erase(remove_if(nums.begin(), nums.end(), [target](const int num){ return num > target; }), nums.end());
        sort(nums.begin(), nums.end());
        int ans = 0, n = nums.size();
        vector<int> dp(target + 1, 0);
        for(int i = 0; i < n; ++i){
            dp[nums[i]] = 1;
        }
        for(int num = 1; num <= target; ++num){
            for(int i = 0; i < n && num >= nums[i]; ++i){
                if(INT_MAX - dp[num] < dp[num - nums[i]]){
                    dp[num] = 0;
                }else{
                    dp[num] += dp[num - nums[i]];
                }
            }
        }
        return dp[target];
    }
};
// @lc code=end

