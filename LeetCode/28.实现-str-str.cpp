/*
 * @lc app=leetcode.cn id=28 lang=cpp
 *
 * [28] 实现 strStr()
 */

// @lc code=start
class Solution {
public:
    int strStr(string haystack, string needle) {
        int m = haystack.size(), n = needle.size();
        if(m < n) return -1;
        if(n == 0) return 0;
        for(int i = 0; i <= m - n; ++i){
            if(haystack[i] == needle[0] && match(haystack, needle, i)){
                return i;
            }
        }
        return -1;
    }
    bool match(string s1, string s2, int pos){
        if(pos >= s1.size()) return false;
        for(int i = 0; i < s2.size(); ++i){
            if(s1[pos + i] != s2[i])
                return false;
        }
        return true;
    }
};
// @lc code=end

