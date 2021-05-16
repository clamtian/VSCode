// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem15.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        string s(3, "01");
        vector<vector<int>> son;
        int n = nums.size();
        vector<int> v = {};
        son.push_back(v);
        for(int i = 0; i < n; ++i){
            for(auto vec : son){
                vec.push_back(nums[i]);
                son.push_back(vec);
            }
        }
        int m = son.size(), ans = 0;
        for(int i = 0; i < m; ++i){
            int xo = 0, len = son[i].size();
            for(int j = 0; j < len; ++j){
                xo = xo ^ son[i][j];
            }
            ans += xo;
        }
        return son;
    }
};
// @lc code=end

