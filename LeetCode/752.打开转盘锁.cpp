// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include "commoncppproblem752.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=752 lang=cpp
 *
 * [752] 打开转盘锁
 */

// @lc code=start
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        if(target == "0000") return 0;
        queue<pair<string, int>> q;
        q.push({ "0000", 0 });
        unordered_set<string> uset(deadends.begin(), deadends.end());
        while(!q.empty()){
            auto [loc, step] = q.front();
            q.pop();
            vector<string> cand = getCand(loc);
            for(string s : cand){
                if(s == target) return step + 1;
                if(uset.find(s) == uset.end()){
                    q.push({ s, step + 1 });
                    uset.insert(s);
                }
            }
        }
        return -1;
    }
    vector<string> getCand(string s){
        vector<string> res;
        string t = s;
        t[0] = t[0] == '9' ? '0' : t[0] + '1';
        res.push_back(t);
        t = s;
        t[0] = t[0] == '0' ? '9' : t[0] - '1';
        res.push_back(t);
        t = s;
        t[1] = t[1] == '9' ? '0' : t[1] + '1';
        res.push_back(t);
        t = s;
        t[1] = t[1] == '0' ? '9' : t[1] - '1';
        res.push_back(t);
        t = s;
        t[2] = t[2] == '9' ? '0' : t[2] + '1';
        res.push_back(t);
        t = s;
        t[2] = t[2] == '0' ? '9' : t[2] - '1';
        res.push_back(t);
        t = s;
        t[3] = t[3] == '9' ? '0' : t[3] + '1';
        res.push_back(t);
        t = s;
        t[3] = t[3] == '0' ? '9' : t[3] - '1';
        res.push_back(t);

        return res;
    }
    
};
// @lc code=end

