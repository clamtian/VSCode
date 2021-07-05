// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <map>
#include <stack>
#include "commoncppproblem726.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=726 lang=cpp
 *
 * [726] 原子的数量
 */

// @lc code=start
class Solution {
public:
    string countOfAtoms(string f) {
        int n = f.size(), pos = 0;
        stack<map<string, int>> s;
        s.push(map<string, int>());
        while(pos < n){
            if(f[pos] == '('){
                s.push(map<string, int>());
                ++pos;
            }else if(f[pos] == ')'){
                pos += 1;
                int pos_ = pos;
                int mul = 1;
                while(pos_ < n && (f[pos_] >= '0' && f[pos_] <= '9')) ++pos_;
                if(pos_ > pos) mul = stoi(f.substr(pos, pos_ - pos));
                auto m = s.top();
                s.pop();
                auto& m1 = s.top();
                for(auto& [atom, num] : m){
                    m1[atom] += num * mul;
                }
                pos = pos_;
            }else{
                int pos_ = pos + 1;
                while(pos_ < n && (f[pos_] >= 'a' && f[pos_] <= 'z')) ++pos_;
                auto& m = s.top();
                string atom = f.substr(pos, pos_ - pos);
                pos = pos_;
                while(pos_ < n && (f[pos_] >= '0' && f[pos_] <= '9')) ++pos_;
                if(pos_ > pos) m[atom] += stoi(f.substr(pos, pos_ - pos));
                else m[atom] += 1;
                pos = pos_;
            }
        }
        auto& m = s.top();
        string res = "";
        for(auto& [atom, num] : m){
            res += atom;
            if(num > 1) res += to_string(num);
        }
        return res;
    }
};
// @lc code=end

