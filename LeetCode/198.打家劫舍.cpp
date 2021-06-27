/*
 * @lc app=leetcode.cn id=198 lang=cpp
 *
 * [198] 打家劫舍
 */

// @lc code=start
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if(n == 1) return nums[0];
        int dp1 = nums[0], dp2 = max(nums[1], nums[0]);
        for(int i = 2; i < n; ++i){
            int tmp = 0;
            if(dp1 == dp2) tmp = dp2 + nums[i];
            else tmp = max(dp1 + nums[i], dp2);
            dp1 = dp2;
            dp2 = tmp;
        }
        return max(dp1, dp2);
    }
};
// @lc code=end

