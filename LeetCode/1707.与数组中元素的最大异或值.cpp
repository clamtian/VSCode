/*
 * @lc app=leetcode.cn id=1707 lang=cpp
 *
 * [1707] 与数组中元素的最大异或值
 */

// @lc code=start
class Solution {
public:
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        int n = queries.size();
        vector<int> res(n, -1), tmp(n, 0), flag(n, 0);
        unordered_map<int, int> m;
        for(int k = 30; k >= 0; --k){
            for(int num : nums){
                if(m.find(num >> k) != m.end()){
                    m[num >> k] = min(m[num >> k], num);
                }else{
                    m[num >> k] = num;
                }
            }
            for(int i = 0; i < n; ++i){
                if((queries[i][0] >> k) > 0 || (queries[i][1] >> k) > 0){
                    int target = (tmp[i] * 2 + 1) ^ (queries[i][0] >> k);
                    if(m.find(target) != m.end() && m[target] <= queries[i][1]){
                        tmp[i] = tmp[i] * 2 + 1;
                        flag[i] = 1;
                    }else{
                        tmp[i] = tmp[i] * 2;
                    }
                }
            }
            m.clear();
        }
        unordered_set<int> s(nums.begin(), nums.end());
        for(int i = 0; i < n; ++i){
            int val = queries[i][0] ^ tmp[i];
            if(s.find(val) != s.end() && val <= queries[i][1]){
                res[i] = tmp[i];
            }
        }
        return res;
    }
};
// @lc code=end

