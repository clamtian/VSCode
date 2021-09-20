// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem673.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=673 lang=cpp
 *
 * [673] 最长递增子序列的个数
 */

// @lc code=start
typedef pair<int, int> PII;
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        vector<PII> dp(n, { 1, 1 });
        for(int i = 1; i < n; ++i){
            int maxLen = 0, time = 0;
            for(int j = 0; j < i; ++j){
                if(nums[i] > nums[j] && dp[j].first > maxLen){
                    maxLen = dp[j].first;
                    time = 1;
                }else if(nums[i] > nums[j] && dp[j].first == maxLen){
                    time += dp[j].second;
                }
            }
            if(maxLen) dp[i] = { maxLen + 1, time };
        }
        int ans = 0, maxLen = 0;
        for(auto [len, num] : dp){
            if(maxLen < len)
                maxLen = len, ans = num;
            else if(maxLen == len) ans += num;
        }
        return ans;
    }
};
// @lc code=end

