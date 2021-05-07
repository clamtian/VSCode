// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <map>
#include "commoncppproblem740.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=740 lang=cpp
 *
 * [740] 删除与获得点数
 */

// @lc code=start
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        map<int, int> m;
        for(int num : nums){
            m[num] += num;
        }
        auto it = m.begin(), it_ = m.begin();
        int dp1 = 0, dp2 = 0, ans = 0;
        while(it != m.end()){
            int tmp = dp2;
            dp2 = max(dp2, dp1);
            dp1 = tmp + it->second;
            it_ = it;
            ++it;
            if(it != m.end() && it->first != it_->first + 1){
                ans += max(dp1, dp2);
                dp1 = dp2 = 0;
            }
        }
        ans += max(dp1, dp2);
        return ans;
    }
};
// @lc code=end

