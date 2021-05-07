// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <numeric>
#include "commoncppproblem517.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=517 lang=cpp
 *
 * [517] 超级洗衣机
 */

// @lc code=start
class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        
        int ans = 0, n = machines.size();
        vector<int> dp(n + 1, 0);
        for(int i = 1; i <= n; ++i){
            dp[i] = dp[i - 1] + machines[i - 1];
        }
        if(dp[n] % n != 0) return -1;
        int ave = dp[n] / n;
        for(int i = 0; i < n; ++i){
            ans = max(ans, machines[i] - ave);
            ans = max(ans, abs(ave * (i + 1) - dp[i + 1]));
        }
        return ans;
    }
};
// @lc code=end

