/*
 * @lc app=leetcode.cn id=1738 lang=cpp
 *
 * [1738] 找出第 K 大的异或坐标值
 */

// @lc code=start
class Solution {
public:
    int kthLargestValue(vector<vector<int>>& matrix, int k) {
        vector<int> vec;
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> xor_vec(m + 1, vector<int>(n + 1, 0));
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                xor_vec[i + 1][j + 1] = xor_vec[i][j + 1] ^ xor_vec[i + 1][j] ^ xor_vec[i][j] ^ matrix[i][j];
            }
        }
        for(int i = 1; i <= m; ++i){
            for(int j = 1; j <= n; ++j){
                vec.push_back(xor_vec[i][j]);
            }
        }
        sort(vec.begin(), vec.end());
        int pos = m * n;
        while(k > 0){
            --pos;
            --k;
        }
        return vec[pos];
    }
};
// @lc code=end

