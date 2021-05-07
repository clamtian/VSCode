// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem546.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=546 lang=cpp
 *
 * [546] 移除盒子
 */

// @lc code=start
class Solution {
public:
    vector<vector<vector<int>>> dp;
    int removeBoxes(vector<int>& boxes) {
        int n = boxes.size();
        dp.resize(n, vector<vector<int>>(n, vector<int>(n, 0)));
        return calculate(boxes, 0, n - 1, 0);
    }
    int calculate(vector<int>& boxes, int l, int r, int k){
        if(l > r) return 0;
        if(dp[l][r][k] == 0){
            dp[l][r][k] = calculate(boxes, l, r - 1, 0) + (k + 1) * (k + 1);
            for(int i = l; i < r; ++i){
                if(boxes[i] == boxes[r]){
                    dp[l][r][k] = max(dp[l][r][k], calculate(boxes, l, i, k + 1) + calculate(boxes, i + 1, r - 1, 0));
                }
            }
        }
        return dp[l][r][k];
    }
};
// @lc code=end

