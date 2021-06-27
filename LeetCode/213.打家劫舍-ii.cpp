/*
 * @lc app=leetcode.cn id=213 lang=cpp
 *
 * [213] 打家劫舍 II
 */

// @lc code=start
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if(n == 1) return nums[0];
        if(n == 2) return max(nums[0], nums[1]);
        int dp1 = nums[0], dp2 = max(nums[0], nums[1]), ans = max(dp1, dp2);
        for(int i = 2; i < n - 1; ++i){
            if(dp1 == dp2) ans = dp2 + nums[i];
            else ans = max(dp1 + nums[i], dp2);
            dp1 = dp2;
            dp2 = ans;
        }
        int cmp = ans;
        dp1 = nums[1], dp2 = max(nums[1], nums[2]);
        for(int i = 3; i < n; ++i){
            if(dp1 == dp2) ans = dp2 + nums[i];
            else ans = max(dp1 + nums[i], dp2);
            dp1 = dp2;
            dp2 = ans;
        }
        return max(ans, cmp);
    }
};
// @lc code=end

