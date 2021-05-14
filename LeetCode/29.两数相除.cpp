// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem29.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=29 lang=cpp
 *
 * [29] 两数相除
 */

// @lc code=start
class Solution {
public:
    int divide(int dividend, int divisor) {
        if(dividend < divisor) return 0;
        int flag = 0;
        if(dividend * dividend < 0) flag = 1;
        dividend = abs(dividend);
        divisor = abs(divisor);
        int left = divisor, right = dividend;
        while(left < right){
            int mid = left + (right - left + 1) / 2;
            if(mid * divisor > dividend){
                right = mid - 1;
            }else{
                left = mid;
            }
        }
        return flag ? -left : left;
    }
};
// @lc code=end

