// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <algorithm>
#include "commoncppproblem632.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=632 lang=cpp
 *
 * [632] 最小区间
 */

// @lc code=start
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int n = nums.size();
        vector<int> point(n);
        auto cm = [&](const int a, const int b){
            return nums[a][point[a]] > nums[b][point[b]];
        };
        priority_queue<int, vector<int>, decltype(cm)> pq(cm);
        int l = INT_MAX, r = INT_MIN;
        for(int i = 0; i < n; ++i){
            pq.push(i);
            l = min(l, nums[i][0]);
            r = max(r, nums[i][0]);
        }
        while(pq.size()){
            int i = pq.top();
            pq.pop();
            ++point[i];
            if(point[i] == nums[i].size()) break;
            int q = max(r, nums[i][point[i]]);
            pq.push(i);
            int p = nums[pq.top()][point[pq.top()]];
            if(r - l > q - p) r = q, l = p;
            else if(r - l == q - p && l > p)  r = q, l = p;
        }
        return { l, r };
    }
};
// @lc code=end

