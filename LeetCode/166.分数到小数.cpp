// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem166.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=166 lang=cpp
 *
 * [166] 分数到小数
 */

// @lc code=start
class Solution {
public:
    string fractionToDecimal(int n, int d) {
        if(n % d == 0) return to_string(n / d);
        int cnt = 0, flag = 1;
        unordered_map<int, int> umap;
        string ans = "";
        while(n){
            if(umap.find(n) != umap.end()){
                    int idx = umap[n];
                    ans.insert(idx, 1, '(');
                    ans.push_back(')');
                    break;
                }
            if(n < d){
                if(flag) ans.push_back('.'), flag = 0;
                else if(cnt > 0) 
                    ans.push_back('0'), umap[n] = ans.size() - 1;
                n *= 10;
                ++cnt;
            }else{
                cnt = 0;
                
                ans.push_back(n / d + '0');
                n %= d;
                umap[n] = ans.size() - 1;
            }
        }
        if(ans[0] == '.') ans.insert(0, 1, '0');
        return ans;
    }
};
// @lc code=end

