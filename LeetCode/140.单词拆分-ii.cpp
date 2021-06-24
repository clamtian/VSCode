// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include "commoncppproblem140.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=140 lang=cpp
 *
 * [140] 单词拆分 II
 */

// @lc code=start
class Solution {
public:
    vector<vector<string>> container;
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        int n = s.size();
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        unordered_set<string> uset(wordDict.begin(), wordDict.end());
        vector<string> vec;
        dfs(s, uset, vec, 0);
        vector<string> res;
        for(auto& vec : container){
            string t = "";
            for(string r : vec) t += r + " ";
            t.pop_back();
            res.push_back(t);
        }
        return res;
    }
    void dfs(string s, unordered_set<string>& uset, vector<string>& vec, int pos){
        int n = s.size();
        if(pos == n) container.push_back(vec);
        for(int i = pos; i < n; ++i){
            if(uset.find(s.substr(pos, i - pos + 1)) != uset.end()){
                vec.push_back(s.substr(pos, i - pos + 1));
                dfs(s, uset, vec, i + 1);
                vec.pop_back();
            }
        }
    }
};
// @lc code=end

