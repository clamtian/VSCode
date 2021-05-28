/*
 * @lc app=leetcode.cn id=477 lang=cpp
 *
 * [477] 汉明距离总和
 */

// @lc code=start
class Solution {
public:
    int totalHammingDistance(vector<int>& nums) {
        int ans = 0, num0 = 0, num1 = 0;
        for(int k = 30; k >= 0; --k){
            for(int num : nums){
                if((num >> k) & 1 == 1){
                    ++num1;
                }else{
                    ++num0;
                }
            }
            ans += num1 * num0;
            num1 = num0 = 0;
        }
        return ans;
    }
};
// @lc code=end

