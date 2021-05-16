// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem576.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=576 lang=cpp
 *
 * [576] 出界的路径数
 */

// @lc code=start
class Solution {
public:
    int MOD = 1e9 + 7;
    vector<vector<vector<long long>>> dp;
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        dp.resize(m, vector<vector<long long>>(n, vector<long long>(maxMove + 1, -1)));
        return dfs(m, n, maxMove, startRow, startColumn);
    }
    long long dfs(int m, int n, int move, int i, int j){
        if(i == -1 || i == m || j == -1 || j == n) return 1;
        if(move == 0) return 0;
        if(dp[i][j][move] != -1) return dp[i][j][move];
        if(i == 0 && j == 0){
            dp[i][j][move] = 2 + dfs(m, n, move - 1, i + 1, j) + dfs(m, n, move - 1, i, j + 1);
        }else if(i == 0 && j == n - 1){
            dp[i][j][move] = 2 + dfs(m, n, move - 1, i + 1, j) + dfs(m, n, move - 1, i, j - 1);
        }else if(i == m - 1 && j == 0){
            dp[i][j][move] = 2 + dfs(m, n, move - 1, i - 1, j) + dfs(m, n, move - 1, i, j + 1);
            
        }else if(i == m - 1 && j == n - 1){
            dp[i][j][move] = 2 + dfs(m, n, move - 1, i - 1, j) + dfs(m, n, move - 1, i, j - 1);
            
        }else if(i == 0){
            dp[i][j][move] = 1 + dfs(m, n, move - 1, i, j + 1) + dfs(m, n, move - 1, i, j - 1) + dfs(m, n, move - 1, i + 1, j);
            
        }else if(i == m - 1){
            dp[i][j][move] = 1 + dfs(m, n, move - 1, i, j + 1) + dfs(m, n, move - 1, i, j - 1) + dfs(m, n, move - 1, i - 1, j);
            
        }else if(j == 0){
            dp[i][j][move] = 1 + dfs(m, n, move - 1, i, j + 1) + dfs(m, n, move - 1, i + 1, j) + dfs(m, n, move - 1, i - 1, j);
            
        }else if(j == n - 1){
            dp[i][j][move] = 1 + dfs(m, n, move - 1, i - 1, j) + dfs(m, n, move - 1, i + 1, j) + dfs(m, n, move - 1, i, j - 1);
            
        }else{
            dp[i][j][move] = dfs(m, n, move - 1, i - 1, j) + dfs(m, n, move - 1, i + 1, j) + dfs(m, n, move - 1, i, j - 1) + dfs(m, n, move - 1, i, j + 1);
        }
        dp[i][j][move] %= MOD;
        return dp[i][j][move];
    }
};
// @lc code=end

