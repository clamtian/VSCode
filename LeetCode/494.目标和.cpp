/*
 * @lc app=leetcode.cn id=494 lang=cpp
 *
 * [494] 目标和
 */

// @lc code=start
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int n = nums.size();
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if(abs(target) > sum) return 0;
        vector<int> dp1(2 * sum + 1, 0), dp2(2 * sum + 1, 0);
        dp1[nums[0] + sum] = 1;
        dp1[sum - nums[0]] += 1;
        for(int i = 1; i < n; ++i){
            for(int j = 2 * sum - nums[i]; j >= nums[i]; --j){
                dp2[j + nums[i]] += dp1[j];
                dp2[j - nums[i]] += dp1[j];
            }  
            swap(dp1, dp2);  
            dp2 = vector<int>(2 * sum + 1, 0);
        }
        return dp1[target + sum];
    }
};
// @lc code=end

