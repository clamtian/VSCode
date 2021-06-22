/*
 * @lc app=leetcode.cn id=1301 lang=cpp
 *
 * [1301] 最大得分的路径数目
 */

// @lc code=start
typedef long long LL;
int MOD = 1e9 + 7;
class Solution {
public:
    vector<int> pathsWithMaxScore(vector<string>& board) {
        vector<int> res = { 0, 0 };
        int n = board.size();
        vector<vector<LL>> val(n, vector<LL>(n, 0));
        vector<vector<LL>> path(n, vector<LL>(n, 0));
        path[n - 1][n - 1] = 1;
        for(int i = n - 2; i >= 0; --i){
            if(board[i][n - 1] != 'X'){
                val[i][n - 1] = val[i + 1][n - 1] + (board[i][n - 1] - '0');
                path[i][n - 1] = 1;
            }else{
                for(; i >= 0; --i){
                    val[i][n - 1] = -1;
                    path[i][n - 1] = 0;
                }
            }     
        }
        for(int j = n - 2; j >= 0; --j){
            if(board[n - 1][j] != 'X'){
                val[n - 1][j] = val[n - 1][j + 1] + (board[n - 1][j] - '0');
                path[n - 1][j] = 1;
            }else{
                for(; j >= 0; --j){
                    val[n - 1][j] = -1;
                    path[n - 1][j] = 0;
                }
            }     
        }
        for(int i = n - 2; i >= 0; --i){
            for(int j = n - 2; j >= 0; --j){
                if(board[i][j] != 'X'){
                    if(path[i + 1][j] > 0){
                        val[i][j] = val[i + 1][j];
                        path[i][j] = path[i + 1][j];
                    }
                    if(path[i][j + 1] > 0){
                        if(val[i][j + 1] > val[i][j]){
                            val[i][j] = val[i][j + 1];
                            path[i][j] = path[i][j + 1];
                        }else if(val[i][j + 1] == val[i][j]){
                            path[i][j] += path[i][j + 1];
                        }
                    }
                    if(path[i + 1][j + 1] > 0){
                        if(val[i + 1][j + 1] > val[i][j]){
                            val[i][j] = val[i  +1][j + 1];
                            path[i][j] = path[i + 1][j + 1];
                        }else if(val[i + 1][j + 1] == val[i][j]){
                            path[i][j] += path[i + 1][j + 1];
                        }
                    }
                    if(board[i][j] != 'E') val[i][j] += (board[i][j] - '0');
                    if(path[i][j] >= MOD) path[i][j] %= MOD;
                }else if(board[i][j] != 'E'){
                    val[i][j] = -1;
                    path[i][j] = 0;
                }
            }
        }
        if(path[0][0] == 0) return res;
        res[0] = val[0][0] % MOD;
        res[1] = path[0][0] % MOD;
        return res;
    }
};
// @lc code=end

