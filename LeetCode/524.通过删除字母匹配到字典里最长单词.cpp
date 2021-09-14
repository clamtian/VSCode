// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem524.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=524 lang=cpp
 *
 * [524] 通过删除字母匹配到字典里最长单词
 */

// @lc code=start
class Solution {
public:
    string findLongestWord(string s, vector<string>& dict) {
        int n = dict.size();
        string ans = "";
        for(int i = 0; i < n; ++i){
            if(match(s, dict[i]) && cmp(dict[i], ans))
                ans = dict[i]; 
        }
        return ans;
    }
    bool cmp(string& s, string& t){
        int m = s.size(), n = t.size();
        if(m < n) return false;
        return s < t;
    }
    bool match(string& s, string& t){
        int n = s.size(), m = t.size();
        int i = 0, j = 0;
        while(i + m - j <= n && j < m){
            if(s[i] == t[j]) ++j;
            ++i;
        }
        return j == m;
    }
};
// @lc code=end

