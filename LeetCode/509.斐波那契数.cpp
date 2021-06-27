/*
 * @lc app=leetcode.cn id=509 lang=cpp
 *
 * [509] 斐波那契数
 */

// @lc code=start
class Solution {
public:
    int fib(int n) {
        if(n < 2) return n;
        int a = 0, b = 0, ans = 1;
        while(n >= 2){
            a = b;
            b = ans;
            ans = a + b;
            --n;
        }
        return ans;
    }
};
// @lc code=end

