// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem165.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=165 lang=cpp
 *
 * [165] 比较版本号
 */

// @lc code=start
class Solution {
public:
    int compareVersion(string version1, string version2) {
        int m = version1.size(), n = version2.size();
        int p1 = 0, p2 = 0;
        while(p1 < m && p2 < n){
            int i = p1 + 1, j = p2 + 1;
            while(i < m && version1[i] != '.') ++i;
            while(j < n && version2[j] != '.') ++j;
            int v1 = stoi(version1.substr(p1 + 1, i));
            int v2 = stoi(version2.substr(p2 + 1, j));
            if(v1 > v2) return 1;
            else if(v1 < v2) return -1;
            p1 = i, p2 = j;
        }
        while(p1 < m){
            int i = p1 + 1;
            while(i < m && version1[i] != '.') ++i;
            int v1 = stoi(version1.substr(p1 + 1, i));
            if(v1 > 0) return 1;
            p1 = i;
        }
        while(p2 < n){
            int j = p2 + 1;
            while(j < n && version2[j] != '.') ++j;
            int v2 = stoi(version2.substr(p2 + 1, j));
            if(v2 > 0) return -1;
            p2 = j;
        }
        return 0;
    }
};
// @lc code=end

