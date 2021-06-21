/*
 * @lc app=leetcode.cn id=119 lang=cpp
 *
 * [119] 杨辉三角 II
 */

// @lc code=start
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> res = { 1 };
        for(int i = 1; i <= rowIndex; ++i){
            res.push_back(1);
            for(int j = i - 1; j > 0; --j){
                res[j] += res[j - 1];
            }
        }
        return res;
    }
};
// @lc code=end

