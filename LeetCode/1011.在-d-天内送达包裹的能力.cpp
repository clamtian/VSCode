// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include "commoncppproblem1011.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=1011 lang=cpp
 *
 * [1011] 在 D 天内送达包裹的能力
 */

// @lc code=start
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int D) {
        int n = weights.size();
        int maxVal = *max_element(weights.begin(), weights.end());
        int tot = accumulate(weights.begin(), weights.end(), 0);
        int l = maxVal, r = tot;
        while(l < r){
            int mid = l + (r - l) / 2;
            if(canComplete(weights, D, mid)){
                r = mid;
            }else{
                l = mid + 1;
            }
        }
        return l;
    }
    bool canComplete(vector<int>& weights, int D, int load){
        int pos = 0;
        while(D > 0 && pos < weights.size()){
            int sum = 0;
            while(pos < weights.size() && sum + weights[pos]<= load){
                sum += weights[pos];
                ++pos;
            }
            if(pos == weights.size()) return true;
            sum = 0;
            --D;
        }
        return false;
    }
};
// @lc code=end

