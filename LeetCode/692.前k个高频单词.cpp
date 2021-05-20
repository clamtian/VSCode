// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem692.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=692 lang=cpp
 *
 * [692] 前K个高频单词
 */

// @lc code=start
bool compare(const pair<string, int>& p1, const pair<string, int>& p2){
    if(p1.second == p2.second){
        return p1.first < p2.first;
    }
    return p1.second > p2.second;
}
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> m;
        for(string& word : words){
            ++m[word];
        }
        vector<pair<string, int>> vec(m.begin(), m.end());
        sort(vec.begin(), vec.end(), compare);
        vector<string> res;
        for(int i = 0; i < k; ++i){
            res.push_back(vec[i].first);
        }
        return res;
    }
};
// @lc code=end

