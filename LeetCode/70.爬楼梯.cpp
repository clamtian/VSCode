/*
 * @lc app=leetcode.cn id=70 lang=cpp
 *
 * [70] 爬楼梯
 */

// @lc code=start
class Solution {
public:
    int climbStairs(int n) {
        if(n < 3) return n;
        int a = 0, b = 1, ans = 2;
        while(n > 2){
            a = b;
            b = ans;
            ans = a + b;
            --n;
        }
        return ans;
    }
};
// @lc code=end

