/*
 * @lc app=leetcode.cn id=139 lang=cpp
 *
 * [139] 单词拆分
 */

// @lc code=start
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> uset(wordDict.begin(), wordDict.end());
        int n = s.size();
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for(int i = 1; i <= n; ++i){
            for(int j = i - 1; j>= 0; --j){
                if(dp[j] && uset.find(s.substr(j, i - j)) != uset.end()){
                    dp[i] = 1;
                    break;
                }
            }
        }
        return dp[n];
    }
};
// @lc code=end

