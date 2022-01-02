// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <set>
#include "commoncppproblem301.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=301 lang=cpp
 *
 * [301] 删除无效的括号
 */

// @lc code=start
class Solution {
public:
    set<string> res;
    vector<string> removeInvalidParentheses(string s) {
        int n = s.size();
        int l = 0, r = 0;
        for(int i = 0; i < n; ++i){
            if(s[i] == '(') ++l;
            else if(s[i] == ')') 
                if(l) --l;
                else ++r;
        }
        string t;
        dfs(s, l, r, 0, 0, t);
        vector<string> ans(res.begin(), res.end());
        return ans;
    }
    void dfs(string& s, int l, int r, int re, int p, string& t){
        if(p == s.size()){
            if(!l && !r && !re) cout << t << endl, res.insert(t);
            return;
        }
        if(s[p] == '('){
            if(l) dfs(s, l - 1, r, re, p + 1, t);
            t.push_back(s[p]);
            dfs(s, l, r, re + 1, p + 1, t);
            t.pop_back();
        }else if(s[p] == ')'){
            if(r) dfs(s, l, r - 1, re, p + 1, t);
            if(re){
                t.push_back(s[p]);
                dfs(s, l, r, re - 1, p + 1, t);
                t.pop_back();
            }else return;
        }else{
            t.push_back(s[p]);
            dfs(s, l, r, re, p + 1, t);
        }
    }
};
// @lc code=end

