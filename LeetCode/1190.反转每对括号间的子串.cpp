/*
 * @lc app=leetcode.cn id=1190 lang=cpp
 *
 * [1190] 反转每对括号间的子串
 */

// @lc code=start
class Solution {
public:
    string reverseParentheses(string s) {
        int n = s.size();
        vector<int> pos(n, 0);
        stack<int> st;
        for(int i = 0; i < n; ++i){
            if(s[i] == '('){
                st.push(i);
            }else if(s[i] == ')'){
                pos[i] = st.top();
                pos[st.top()] = i;
                st.pop();
            }
        }
        string res = "";
        int dir = 1;
        for(int i = 0; i < n; i += dir){
            if(s[i] == '(' || s[i] == ')'){
                i = pos[i];
                dir = -dir;
            }else{
                res += s[i];
            }
        }
        return res;
    }
};
// @lc code=end

