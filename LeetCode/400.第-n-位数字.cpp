// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem400.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=400 lang=cpp
 *
 * [400] 第 N 位数字
 */

// @lc code=start
class Solution {
public:
    int findNthDigit(int n) {
        long long l = 1, r = n;
        while(l < r){
            long long mid = (l + r) >> 1;
            int flag = detector(mid, n);
            if(flag == -1) l = mid + 1;
            else if(flag == -2) r = mid;
            else return flag;
        }
        return detector(l, n);
    }
    int detector(long long num, long long n){
        long long k = 1, cnt = 0;
        long long sum = 0;
        while(k <= num){
            sum = sum + (k - k / 10) * cnt++;
            k *= 10;
        }
        //cout << k << endl;
        //cout << cnt << endl;
        sum += (num - k / 10) * cnt ;
        //cout << sum << endl;
        if(n > sum){
            if(n <= sum + cnt){
                int dive = 10 * (cnt - n + sum);
                if(dive > 0) num /= dive;
                return num % 10;
            }else return -1;
        }
        return -2;
    }
};
// @lc code=end

