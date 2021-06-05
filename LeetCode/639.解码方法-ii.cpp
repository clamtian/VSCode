/*
 * @lc app=leetcode.cn id=639 lang=cpp
 *
 * [639] 解码方法 II
 */

// @lc code=start
class Solution {
public:
    int MOD = 1e9 + 7;
    int numDecodings(string s) {
        int n = s.size();
        if(s[0] == '0') return 0;
        vector<long long> dp1(4, 0), dp2(4, 0);// ['1']['2']['3-9'][total]
        dp2[3] = 1;
        if(s[0] == '1'){
            dp1[0] = 1;
            dp1[3] = 1;
        }else if(s[0] == '2'){
            dp1[1] = 1;
            dp1[3] = 1;
        }else if(s[0] == '*'){
            dp1[0] = dp1[1] = 1;
            dp1[2] = 7;
            dp1[3] = 9;
        }else{
            dp1[2] = 1;
            dp1[3] = 1;
        }
        for(int i = 1; i < n; ++i){
            vector<long long> vec(4, 0);
            if(s[i - 1] == '0'){
                if(s[i] == '0') return 0;
                if(s[i] == '1' || s[i] == '2'){
                    vec[s[i] - '1'] = dp1[3];
                }else if(s[i] == '*'){
                    vec[0] = dp1[3];
                    vec[1] = dp1[3];
                    vec[2] = 7 * dp1[3];
                }else{
                    vec[2] = dp1[3];
                }
                vec[3] = vec[0] + vec[1] + vec[2];
            }else if(s[i - 1] == '1'){
                if(s[i] == '0') vec[3] = dp2[3];
                else if(s[i] == '*'){
                    vec[0] = dp1[3] + dp2[3];
                    vec[1] = dp1[3] + dp2[3];
                    vec[2] = 7 * (dp1[3] + dp2[3]);
                    vec[3] = vec[0] + vec[1] + vec[2];
                }else if(s[i] == '1' || s[i] == '2'){
                    vec[s[i] - '1'] = dp1[3] + dp2[3];
                    vec[3] = vec[0] + vec[1];
                }else{
                    vec[2] = dp1[3] + dp2[3];
                    vec[3] = vec[2];
                }
            }else if(s[i - 1] == '2'){
                if(s[i] == '0') vec[3] = dp2[3];
                else if(s[i] == '*'){
                    vec[0] = dp1[3] + dp2[3];
                    vec[1] = dp1[3] + dp2[3];
                    vec[2] = 4 * (dp1[3] + dp2[3]) + 3 * dp1[3];
                    vec[3] = vec[0] + vec[1] + vec[2];
                }else if(s[i] == '1' || s[i] == '2'){
                    vec[s[i] - '1'] = dp1[3] + dp2[3];
                    vec[3] = vec[0] + vec[1];
                }else if(s[i] >= '3' && s[i] <= '6'){
                    vec[2] = dp1[3] + dp2[3];
                    vec[3] = vec[2];
                }else{
                    vec[2] = dp1[3];
                    vec[3] = vec[2];
                }
            }else if(s[i - 1] == '*'){
                if(s[i] == '0') vec[3] = 2 * dp2[3];
                else if(s[i] == '1'){
                    vec[3] = vec[0] = dp1[0] + dp2[3] + dp1[1] + dp2[3] + dp1[2];
                }else if(s[i] == '2'){
                    vec[3] = vec[1] = dp1[0] + dp2[3] + dp1[1] + dp2[3] + dp1[2];
                }else if(s[i] == '*'){
                    vec[0] = dp1[0] + dp2[3] + dp1[1] + dp2[3] + dp1[2];
                    vec[1] = dp1[0] + dp2[3] + dp1[1] + dp2[3] + dp1[2];
                    vec[2] = 7 * (dp1[0] + dp2[3]) + 4 * (dp1[1] + dp2[3]) + 3 * dp1[1] + 7 * dp1[2];
                    vec[3] = vec[0] + vec[1] + vec[2];
                }else if(s[i] >= '3' && s[i] <= '6'){
                    vec[2] = dp1[0] + dp2[3] + dp1[1] + dp2[3] + dp1[2];
                    vec[3] = vec[2];
                }else{
                    vec[2] = dp1[0] + dp2[3] + dp1[1] + dp1[2];
                    vec[3] = vec[2];
                }
            }else{
                if(s[i] == '0') return 0;
                else if(s[i] == '*'){
                    vec[0] = dp1[3];
                    vec[1] = dp1[3];
                    vec[2] = 7 * dp1[3];
                }else if(s[i] == '1' || s[i] == '2'){
                    vec[s[i] - '1'] = dp1[3];
                }else{
                    vec[2] = dp1[3];
                }
                vec[3] = vec[0] + vec[1] + vec[2];
            }
            vec[0] %= MOD;
            vec[1] %= MOD;
            vec[2] %= MOD;
            vec[3] %= MOD;
            dp2 = dp1;
            dp1 = vec;
        }
        return dp1[3];
    }
};
// @lc code=end

