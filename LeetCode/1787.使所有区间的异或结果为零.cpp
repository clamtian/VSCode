/*
 * @lc app=leetcode.cn id=1787 lang=cpp
 *
 * [1787] 使所有区间的异或结果为零
 */

// @lc code=start
class Solution {
public:
    int MAXX = 1 << 10;
    int minChanges(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> dp1(MAXX, n);
        dp1[0] = 0;
        for(int i = 0; i < k; ++i){
            unordered_map<int, int> m;
            int size = 0;
            for(int j = i; j < n; j += k){
                m[nums[j]]++;
                size++;
            }
            int cmp = *min_element(dp1.begin(), dp1.end());
            vector<int> dp2(MAXX, cmp + size);
            for(int j = 0; j < MAXX; ++j){
                for(auto [x, cntx] : m){
                    dp2[j] = min(dp2[j], dp1[x ^ j] - cntx + size);
                }
            }
            swap(dp1, dp2);
        }
        return dp1[0];
    }
};
// @lc code=end

