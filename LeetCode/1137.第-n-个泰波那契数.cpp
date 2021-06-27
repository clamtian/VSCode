/*
 * @lc app=leetcode.cn id=1137 lang=cpp
 *
 * [1137] 第 N 个泰波那契数
 */

// @lc code=start
class Solution {
public:
    int tribonacci(int n) {
        if(n < 2) return n;
        if(n == 2) return 1;
        int a = 0, b = 0, c = 1, ans = 1;
        while(n > 2){
            a = b;
            b = c;
            c = ans;
            ans = a + b + c;
            --n;
        }
        return ans;
    }
};
// @lc code=end

