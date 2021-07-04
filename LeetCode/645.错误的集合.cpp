// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem645.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=645 lang=cpp
 *
 * [645] 错误的集合
 */

// @lc code=start
class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        int n = nums.size();
        int x = 0;
        for(int i = 0; i < n; ++i) x ^= nums[i] ^ (i + 1);
        int o = x & (-x), n1 = 0, n2 = 0;
        for(int i = 0; i < n; ++i){
            if(o ^ nums[i]) n1 ^= nums[i];
            else n2 ^= nums[i];
            if(o ^ (i + 1)) n1 ^= (i + 1);
            else n2 ^= (i + 1);
        }
        for(int num : nums){
            if(num == n1) return vector<int>{ n1, n2 };
            if(num == n2) return vector<int>{ n2, n1 };
        }
        return vector<int>{ 0, 0 };
    }
};
// @lc code=end

