/*
 * @lc app=leetcode.cn id=746 lang=cpp
 *
 * [746] 使用最小花费爬楼梯
 */

// @lc code=start
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        int dp1 = cost[0], dp2 = cost[1];
        for(int i = 2; i < n; ++i){
            int tmp = dp2;
            dp2 = min(dp2, dp1) + cost[i];
            dp1 = tmp;
        }
        return min(dp1, dp2);
    }
};
// @lc code=end

