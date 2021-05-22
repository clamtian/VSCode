// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <map>
#include "commoncppproblem810.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=810 lang=cpp
 *
 * [810] 黑板异或游戏
 */

// @lc code=start
class Solution {
public:
    map<int, int> m;
    bool xorGame(vector<int>& nums) {
        int val = 0, n = nums.size();   
        if(n % 2 == 0) return true; 
        for(int num : nums) {
            val ^= num;
        }
        return val == 0;
    }
};
// @lc code=end

