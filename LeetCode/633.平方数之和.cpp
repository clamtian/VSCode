/*
 * @lc app=leetcode.cn id=633 lang=cpp
 *
 * [633] 平方数之和
 */

// @lc code=start
class Solution {
public:
    bool judgeSquareSum(int c) {
        long l = 0, r = sqrt(c);
        while(l <= r){
            long long num = l * l + r * r;
            if(num == c) return true;
            if(num < c){
                ++l;
            }else{
                --r;
            }
        }
        return false;
    }
};
// @lc code=end

