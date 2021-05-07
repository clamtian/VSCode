// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_map>
#include "commoncppproblem466.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=466 lang=cpp
 *
 * [466] 统计重复个数
 */

// @lc code=start
class Solution {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        unordered_map<int, vector<int>> record;
        int cnt1 = 0, cnt2 = 0, pos1 = 0, pos2 = 0;
        while(cnt1 < n1){
            while(pos1 < s1.size()){
                while(pos1 < s1.size() && pos2 < s2.size()){
                    if(s1[pos1] == s2[pos2]){
                        ++pos2;
                    }
                    ++pos1;
                }
                if(pos2 == s2.size()){
                    cnt2 += 1;
                    pos2 = 0;
                }
            }
            pos1 = 0;
            ++cnt1;
            if(record.find(pos2) == record.end()){
                record[pos2] = { cnt1, cnt2 };
            }else{
                int cir1 = cnt1 - record[pos2][0];
                int cir2 = cnt2 - record[pos2][1];
                cnt2 += cir2 * ((n1 - cnt1) / cir1);
                cnt1 += ((n1 - cnt1) / cir1) * cir1;
            }

        }
        return cnt2 / n2;
    }
};
// @lc code=end

