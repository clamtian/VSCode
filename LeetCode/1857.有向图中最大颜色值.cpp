/*
 * @lc app=leetcode.cn id=1857 lang=cpp
 *
 * [1857] 有向图中最大颜色值
 */

// @lc code=start
class Solution {
public:
    unordered_map<int, vector<int>> umap;
    unordered_set<int> uset;
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        for(auto& edge : edges) umap[edge[0]].push_back(edge[1]);
        int n = colors.size(), ans = 0;
        vector<vector<int>> dp(n, vector<int>(27, 0));
        for(int i = 0; i < n; ++i){
            auto vec = dfs(i, colors, dp);
            uset.clear();
            if(vec[0] == -1) return -1;
            else ans = max(ans, *max_element(vec.begin() + 1, vec.end()));
        }
        return ans;
    }
    vector<int> dfs(int k, string& colors, vector<vector<int>>& dp){
        if(dp[k][0] != 0) return dp[k];
        if(uset.find(k) != uset.end()){
            dp[k][0] = -1;
            return dp[k];
        }
        uset.insert(k);
        for(int node : umap[k]){
            vector<int> vec = dfs(node, colors, dp);
            if(vec[0] == -1){
                dp[k][0] = -1;
                return dp[k];
            }
            for(int i = 1; i < 27; ++i){
                dp[k][i] = max(dp[k][i], vec[i]);
            }
        }
        uset.erase(k);
        dp[k][colors[k] - 'a' + 1]++;
        dp[k][0] = 1;
        return dp[k];
    }
};
// @lc code=end

