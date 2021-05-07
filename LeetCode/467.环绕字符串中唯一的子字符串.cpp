// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem467.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=467 lang=cpp
 *
 * [467] 环绕字符串中唯一的子字符串
 */

// @lc code=start
class Solution {
public:
    int findSubstringInWraproundString(string p) {
        int n = p.size(), ans = 1;
        if(n == 0) return 0;
        unordered_map<char, int> m;
        vector<int> dp(n, 0);
        dp[0] = 1;
        m[p[0]] = 1;
        for(int i = 1; i < n; ++i){
            if(p[i] - p[i - 1] == 1 || (p[i] == 'a' && p[i - 1] == 'z')){
                dp[i] = dp[i - 1] + 1;
            }else{
                dp[i] = 1;
            }
            if(m.find(p[i]) == m.end()){
                ans += dp[i];
                m[p[i]] = dp[i];
            }else if(m[p[i]] < dp[i]){
                ans += dp[i] - m[p[i]];
                m[p[i]] = dp[i];
            }
        }
        return ans;
    }
};
// @lc code=end

