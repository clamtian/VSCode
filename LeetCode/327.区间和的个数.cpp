// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem327.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=327 lang=cpp
 *
 * [327] 区间和的个数
 */

// @lc code=start
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<int> sum(n + 1, 0);
        for(int i = 0; i < n; ++i)
            sum[i + 1] = sum[i] + nums[i];
        return merge(sum, lower, upper, 0, n);
    }
    int merge(vector<int>& sum, int lo, int u, int l, int r){
        if(l >= r) return 0;
        int mid = (l + r) >> 1;
        int cnt = merge(sum, lo, u, l, mid) + merge(sum, lo, u, mid + 1, r);
        int i = l, j = l;
        for(int k = mid + 1; k <= r; ++k){
            while(i <= mid && sum[k] - sum[i] < lo) ++i;
            while(j <= mid && sum[k] - sum[j] <= u) ++j;
            cnt += j - i;
        }
        i = l, j = mid + 1;
        int k = 0;
        vector<int> m(r - l + 1, 0);
        while(i <= mid && j <= r){
            if(sum[i] < sum[j]) m[k++] = sum[i++];
            else m[k++] = sum[j++];
        }
        while(i <= mid) m[k++] = sum[i++];
        while(j <= r) m[k++] = sum[j++];
        for(int i = l; i <= r; ++i)
            sum[i] = m[i - l];
        return cnt;
    }
};
// @lc code=end

