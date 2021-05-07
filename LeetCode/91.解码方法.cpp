/*
 * @lc app=leetcode.cn id=91 lang=cpp
 *
 * [91] 解码方法
 */

// @lc code=start
class Solution {
public:
    int numDecodings(string s) {
        int n = s.size();
        if(s[0] == '0') return 0;
        vector<int> dp(n + 1, 1);
        for(int i = 1; i < n; ++i){
            if(s[i] == '0'){
                if(s[i - 1] == '1' || s[i - 1] == '2'){
                    dp[i + 1] = dp[i - 1];
                }else{
                    return 0;
                }
            }else if(s[i - 1] > '2' || s[i - 1] == '0' || (s[i - 1] == '2' && s[i] > '6')){
                dp[i + 1] = dp[i];
            }else{
                dp[i + 1] = dp[i] + dp[i - 1];
            }
        }
        return dp[n];
    }
};
// @lc code=end

