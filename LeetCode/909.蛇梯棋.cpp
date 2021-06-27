// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include "commoncppproblem909.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=909 lang=cpp
 *
 * [909] 蛇梯棋
 */

// @lc code=start
class Solution {
public:
    unordered_set<int> uset;
    int snakesAndLadders(vector<vector<int>>& board) {
        queue<pair<int, int>> q;
        int n = board.size();
        q.push({ 1, 0 });
        while(!q.empty()){
            auto [loc, step] = q.front();
            q.pop();
            for(int i = 1; loc + i <= n * n && i < 7; ++i){
                if(loc + i == n * n - n + 1) return step + 1;
                int target = loc + i;
                int x = n - 1 - (target - 1) / n;
                int y = ((target - 1) / n) % 2 == 0 ? (target - 1) % n : n - (target - 1) % n - 1;
                if(board[x][y] != -1)
                    target = board[x][y];
                else
                    target = loc + i;
                if(uset.find(target) == uset.end()){
                    q.push({ target, step + 1 });
                    uset.insert(target);
                }
            }
        }
        return -1;
    }
};
// @lc code=end

