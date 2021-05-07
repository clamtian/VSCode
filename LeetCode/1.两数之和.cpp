// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem1.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = nums.size();
        vector<int> res;
        if(n < 2)
            return res;
        unordered_map<int, int> hmap;
        for (int i = 0; i < n; ++i){
            if (hmap.find(target - nums[i]) != hmap.end()){
                res = { hmap[target - nums[i]] , i };
                return res;
            }
            hmap[nums[i]] = i;
        }
        return res;
    }
};
// @lc code=end

