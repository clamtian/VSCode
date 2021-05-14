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
        long _dividend = abs(dividend);
        long _divisor = abs(divisor);
        if(_dividend < _divisor) return 0;
        int flag = 0;
        if((dividend > 0 && divisor < 0) || (dividend < 0 && divisor > 0)) flag = 1;
        long left = 1, right = _dividend;
        while(left < right){
            long mid = left + (right - left + 1) / 2;
            if(mid * _divisor > _dividend){
                right = mid - 1;
            }else{
                left = mid;
            }
        }
        if(flag) left = - left;
        return left > INT_MAX ? INT_MAX : left;
    }
};
// @lc code=end

