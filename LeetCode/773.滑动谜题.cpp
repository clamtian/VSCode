/*
 * @lc app=leetcode.cn id=773 lang=cpp
 *
 * [773] 滑动谜题
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> position = { { 1, 3 }, { 0, 2, 4 }, { 1, 5 }, { 0, 4 }, { 1, 3, 5 }, { 2, 4 } };
    unordered_set<string> uset;
    int slidingPuzzle(vector<vector<int>>& board) {
        string bo = "";
        for(auto& vec : board){
            for(int num : vec) bo += num + '0';
        }
        if(bo == "123450") return 0;
        queue<pair<string, int>> q;
        q.emplace(bo, 0);
        while(!q.empty()){
            auto [status, step] = q.front();
            q.pop();
            int row = find(status);
            for(int p : position[row]){
                string next_status = status;
                swap(next_status[row], next_status[p]);
                if(uset.find(next_status) == uset.end()){
                    if(next_status == "123450") return step + 1;
                    q.emplace(next_status, step + 1);
                    uset.insert(next_status);
                }
            }
        }
        return -1;
    }
    int find(string s){
        for(int i = 0; i < 6; ++i){
            if(s[i] == '0') return i;
        }
        return -1;
    }
};
// @lc code=end

