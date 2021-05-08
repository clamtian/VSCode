/*
 * @lc app=leetcode.cn id=1723 lang=cpp
 *
 * [1723] 完成所有工作的最短时间
 */

// @lc code=start
class Solution {
public:
    int res = INT_MAX;
    int minimumTimeRequired(vector<int>& jobs, int k) {
        sort(jobs.begin(), jobs.end());
        int n = jobs.size();
        vector<int> sum(k, 0);
        sum[0] = jobs[n - 1];
        fill(jobs, sum, n - 2, k - 1, jobs[n - 1]);
        return res;
    }
    void fill(vector<int>& jobs, vector<int>& sum, int pos, int space, int maxSum){
        if(pos == -1 || pos + 1 == space) {
            res = min(res, maxSum);
            return;
        }
        int r = sum.size() - space;
        for(int i = 0; i < r; ++i){
            if(sum[i] + jobs[pos] >= res) continue;
            sum[i] += jobs[pos];
            fill(jobs, sum, pos - 1, space, max(maxSum, sum[i]));
            sum[i] -= jobs[pos];
        }
        if(r < sum.size()){
            sum[r] = jobs[pos];
            fill(jobs, sum, pos - 1, space - 1, max(maxSum, sum[r]));
            sum[r] -= jobs[pos];
        }
        
    }
};
// @lc code=end

