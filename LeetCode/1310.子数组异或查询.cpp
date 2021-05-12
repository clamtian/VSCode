/*
 * @lc app=leetcode.cn id=1310 lang=cpp
 *
 * [1310] 子数组异或查询
 */

// @lc code=start
class Solution {
public:
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        int n = arr.size(), m = queries.size();
        vector<int> dict(n + 1, 0);
        for(int i = 0; i < n; ++i){
            dict[i + 1] = dict[i] ^ arr[i];
        }
        vector<int> res(m, 0);
        for(int i = 0; i < m; ++i){
            res[i] = dict[queries[i][1] + 1] ^ dict[queries[i][0]];
        }
        return res;
    }
};
// @lc code=end

