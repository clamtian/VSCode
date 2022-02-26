// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "commoncppproblem327.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=327 lang=cpp
 *
 * [327] 区间和的个数
 */

// @lc code=start
typedef long long LL;
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        int ans = 0;
        unordered_set<LL> uset;
        vector<LL> sum(n + 1);
        for(int i = 0; i < n; ++i){
            sum[i + 1] = sum[i] + nums[i];
            uset.insert(sum[i + 1]);
            uset.insert(sum[i + 1] - upper);
            uset.insert(sum[i + 1] - lower);
        }
        unordered_map<LL, int> umap;
        int idx = 0;
        for(LL num : uset){
            umap[num] = idx++;
        }
        vector<int> tree(idx + 1);
        for(int i = 0; i <= n; ++i){
            int a = query(umap[sum[i] - upper], tree), b = query(umap[sum[i] - lower] + 1, tree);
            ans += b - a;
            for(int p = umap[sum[i]] + 1; p <= idx; p += p & -p) tree[p] += 1;
        }
        return ans;
    }
    int query(int i, vector<int>& tree){
        int s = 0;
        for(int p = i; p; p -= p & -p) s += tree[p];
        return s;
    }
};
// @lc code=end

