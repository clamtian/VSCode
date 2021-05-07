// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem523.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=523 lang=cpp
 *
 * [523] 连续的子数组和
 */

// @lc code=start
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        if(n < 2) return false;
        unordered_map<int, int> m;
        m[0] = -1;
        int preSum = 0;
        for(int i = 0; i < n; ++i){
            preSum += nums[i];
            int mod = k == 0 ? preSum : preSum % k;
            if(m.find(mod) != m.end()){
                if(i - m[mod] > 1) return true;
            }else{
                m[mod] = i;
            }
        }
        return false;
    }
};
// @lc code=end

