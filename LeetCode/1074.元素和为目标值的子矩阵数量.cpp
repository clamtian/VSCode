// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem1074.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=1074 lang=cpp
 *
 * [1074] 元素和为目标值的子矩阵数量
 */

// @lc code=start
class Solution {
public:
    int numSubmatrixSumTarget(vector<vector<int>>& matrix, int target) {
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> sum(m + 1, vector<int>(n, 0));
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                sum[i + 1][j] = sum[i][j] + matrix[i][j];
            }
        }
        int ans = 0;
        for(int i = 1; i <= m; ++i){
            for(int k = 0; k < i; ++k){
                vector<int> vec(n + 1, 0);
                for(int j = 0; j < n; ++j){
                    vec[j + 1] = sum[i][j] - sum[k][j];
                }
                unordered_map<int, int> m;
                m[0] = 1;
                for(int j = 1; j <= n; ++j){
                    vec[j] += vec[j - 1];
                    if(m.find(vec[j] - target) != m.end()){
                        ans += m[vec[j] - target];
                    }
                    ++m[vec[j]];
                }
            }
        }
        return ans;
    }
};
// @lc code=end

