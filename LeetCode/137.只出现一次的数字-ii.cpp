/*
 * @lc app=leetcode.cn id=137 lang=cpp
 *
 * [137] 只出现一次的数字 II
 */

// @lc code=start
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0;
        for(int i = 0; i < 32; ++i){
            ans << 1;
            int cmp = 1 << i, cnt = 0;
            for(int num : nums){
                if((num & cmp) != 0){
                    ++cnt;
                }
            }
            if(cnt % 3 != 0){
                ans |= cmp;
            }
        }
        return ans;
    }
};
// @lc code=end

