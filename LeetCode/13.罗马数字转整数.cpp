/*
 * @lc app=leetcode.cn id=13 lang=cpp
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
class Solution {
public:
    int romanToInt(string s) {
        int n = s.size();
        int ans = 0, pos = 0;
        while(pos < n){
            switch(s[pos]){
                case 'M' : {
                    ans += 1000;
                    ++pos;
                    break;
                }
                case 'D' : {
                    ans += 500;
                    ++pos;
                    break;
                }
                case 'L' : {
                    ans += 50;
                    ++pos;
                    break;
                }
                case 'V' : {
                    ans += 5;
                    ++pos;
                    break;
                }
                case 'C' : {
                    if(pos < n && s[pos + 1] == 'D'){
                        ans += 400;
                        pos += 2;
                    }else if(pos < n && s[pos + 1] == 'M'){
                        ans += 900;
                        pos += 2;
                    }else{
                        ans += 100;
                        ++pos;
                    }
                    break;
                }
                case 'X' : {
                    if(pos < n && s[pos + 1] == 'L'){
                        ans += 40;
                        pos += 2;
                    }else if(pos < n && s[pos + 1] == 'C'){
                        ans += 90;
                        pos += 2;
                    }else{
                        ans += 10;
                        ++pos;
                    }
                    break;
                }
                case 'I' : {
                    if(pos < n && s[pos + 1] == 'V'){
                        ans += 4;
                        pos += 2;
                    }else if(pos < n && s[pos + 1] == 'X'){
                        ans += 9;
                        pos += 2;
                    }else{
                        ans += 1;
                        ++pos;
                    }
                    break;
                }
            }
        }
        return ans;
    }
};
// @lc code=end

