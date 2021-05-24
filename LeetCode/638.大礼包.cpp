/*
 * @lc app=leetcode.cn id=638 lang=cpp
 *
 * [638] 大礼包
 */

// @lc code=start
class Solution {
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        int n = price.size(), m = special.size(), ans = 0;
        for(int i = 0; i < n; ++i){
            ans += price[i] * needs[i];
        }
        for(int i = 0; i < m; ++i){
            if(cmp(needs, special[i])){
                vector<int> remain = minus(needs, special[i]);
                ans = min(ans, special[i][n] + shoppingOffers(price, special, remain));
            }
        }
        return ans;
    }
    vector<int> minus(vector<int> needs, vector<int>& special){
        int n = needs.size();
        for(int i = 0; i < n; ++i){
            needs[i] -= special[i];
        }
        return needs;
    }
    bool cmp(vector<int>& needs, vector<int>& special){
        int n = needs.size();
        for(int i = 0; i < n; ++i){
            if(special[i] > needs[i])
                return false;
        }
        return true;
    }
};
// @lc code=end

