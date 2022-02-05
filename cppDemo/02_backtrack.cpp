#include <string.h>
#include <vector>
#include <string>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;

/*
 * 给定一个 没有重复 数字的序列，返回其所有可能的全排列。
 * 
 * 示例:
 * 
 * 输入: [1,2,3]
 * 输出:
 * [
 *   [1,2,3],
 *   [1,3,2],
 *   [2,1,3],
 *   [2,3,1],
 *   [3,1,2],
 *   [3,2,1] ]
*/

//没有重复数字，只需记录下标是否用过
class Solution1 {
public:
    vector<vector<int>> res;
    vector<vector<int>> permute(vector<int>& nums) {
        int n = nums.size();
        vector<bool> is_used(n, false);
        vector<int> vec;
        if (n == 0) return res;
        dfs(vec, nums, is_used);
        return res;
    }
    void dfs(vector<int> vec, vector<int>& nums, vector<bool>& is_used) {
        if (vec.size() == nums.size()) {
            res.push_back(vec);
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (!is_used[i]) {
                vector<int> cop = vec;
                cop.push_back(nums[i]);
                is_used[i] = true;
                dfs(cop, nums, is_used);
                is_used[i] = false;
            }
        }
    }
};

/*
 * 给定一个可包含重复数字的序列 nums ，按任意顺序返回所有不重复的全排列。
 * 
 * 示例：
 * 输入：nums = [1,1,2]
 * 输出：
 * [[1,1,2],
 *  [1,2,1],
 *  [2,1,1]]
*/

//有重复数字，需要同时记录数字和下标
class Solution2 {
public:
    vector<vector<int> > res;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        int n = nums.size();
        vector<bool> is_used(n, false);
        vector<int> vec;
        dfs(vec, nums, is_used);
        return res;
    }
    void dfs(vector<int> vec, vector<int>& nums, vector<bool>& is_used) {
        if (vec.size() == nums.size()) {
            res.push_back(vec);
            return;
        }
        unordered_set<int> hashset;
        for (int i = 0; i < nums.size(); ++i) {
            if (!is_used[i] && hashset.find(nums[i]) == hashset.end()) {
                vector<int> cop = vec;
                cop.push_back(nums[i]);
                is_used[i] = true;
                dfs(cop, nums, is_used);
                is_used[i] = false;
                hashset.insert(nums[i]);
            }
        }
    }
};

