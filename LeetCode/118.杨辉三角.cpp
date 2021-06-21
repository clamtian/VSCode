/*
 * @lc app=leetcode.cn id=118 lang=cpp
 *
 * [118] 杨辉三角
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res = { { 1 } };
        for(int i = 1; i < numRows; ++i){
            vector<int> vec(i + 1, 0);
            vec[0] = res[i - 1][0];
            vec[i] = res[i - 1].back();
            for(int j = 1; j < i; ++j){
                vec[j] = res[i - 1][j - 1] + res[i - 1][j];
            }
            res.push_back(vec);
        }
        return res;
    }
};
// @lc code=end

