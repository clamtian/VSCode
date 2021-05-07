// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <algorithm>
#include "commoncppproblem368.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=368 lang=cpp
 *
 * [368] 最大整除子集
 */

// @lc code=start
class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        vector<vector<int>> dp(n);
        dp[0] = { nums[0] };
        int maxAnsSize = 1, maxAnsIndex = 0;
        for(int i = 1; i < n; ++i){
            int maxSize = 0, maxIndex = -1;
            for(int j = 0; j < i; ++j){
                if(nums[i] % nums[j] == 0 && dp[j].size() > maxSize){
                    maxSize = dp[j].size();
                    maxIndex = j;
                }
            }
            if(maxIndex != -1){
                vector vec = dp[maxIndex];
                vec.push_back(nums[i]);
                dp[i] = vec;
            }else{
                dp[i] = { nums[i] };
            }
            if(maxAnsSize < maxSize + 1){
                maxAnsSize = maxSize + 1;
                maxAnsIndex = i;
            }
        }
        return dp[maxAnsIndex];

    }
};
// @lc code=end

