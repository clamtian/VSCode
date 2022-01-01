// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem419.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=419 lang=cpp
 *
 * [419] 甲板上的战舰
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> is_arr;
    int countBattleships(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();
        is_arr = vector<vector<int>>(m, vector<int>(n, 0));
        int ans = 0;
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                if(!is_arr[i][j] && board[i][j] == 'X'){
                    ++ans;
                    find(board, i, j);
                    is_arr[i][j] = 1;
                }
            }
        }
        return ans;
    }
    void find(vector<vector<char>>& board, int i, int j){
        while(i < board.size() && board[i][j] == 'X'){   
            is_arr[i][j] = 1;
            ++i;
        }
        while(j < board[0].size() && board[i][j] == 'X'){
            is_arr[i][j] = 1;
            ++j;
        }
    }
};
// @lc code=end

