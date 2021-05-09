/*
 * @lc app=leetcode.cn id=1482 lang=cpp
 *
 * [1482] 制作 m 束花所需的最少天数
 */

// @lc code=start
class Solution {
public:
    int minDays(vector<int>& bloomDay, int m, int k) {
        int n = bloomDay.size();
        if(n < m * k) return -1;
        int maxDay = *max_element(bloomDay.begin(), bloomDay.end());
        int left = 1, right = maxDay;
        while(left < right){
            int mid = left + (right - left) / 2;
            if(canComplete(bloomDay, m, k, mid)){
                right = mid;
            }else{
                left = mid + 1;
            }
        }
        return left;
    }
    bool canComplete(vector<int>& bloomDay, int m, int k, int day){
        int pos = 0, n = bloomDay.size();
        while(pos < n){
            int k_ = k;
            while(k_ > 0 && pos < n && bloomDay[pos] <= day){
                ++pos;
                --k_;
            }
            if(k_ == 0) --m;
            if(m == 0) return true;
            if(k_ == k) ++pos;
        }
        return false;
    }
};
// @lc code=end

