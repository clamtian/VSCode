/*
 * @lc app=leetcode.cn id=1014 lang=cpp
 *
 * [1014] 最佳观光组合
 */

// @lc code=start
class Solution {
public:
    int maxScoreSightseeingPair(vector<int>& values) {
        int n = values.size();
        vector<int> v(n, 0);
        int lmax = 0, rmax = 0, ans = 0;
        for(int i = 0; i < n; ++i){
            v[i] = lmax + values[i];
            lmax = max(lmax, values[i]) - 1;
        }
        for(int i = n - 1; i >= 0; --i){
            v[i] = max(rmax + values[i], v[i]);
            rmax = max(rmax, values[i]) - 1;
            ans = max(ans, v[i]);
        }
        return ans;
    }
};
// @lc code=end

