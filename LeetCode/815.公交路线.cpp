// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <unordered_set>
#include "commoncppproblem815.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=815 lang=cpp
 *
 * [815] 公交路线
 */

// @lc code=start
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if(source == target) return 0;
        int n = routes.size();
        vector<vector<int>> edge(n, vector<int>(n, 0));
        vector<int> uset(505, 0);
        unordered_map<int, vector<int>> rec;
        for(int i = 0; i < n; ++i){
            for(int site : routes[i]){
                for(int j : rec[site]){
                    edge[i][j] = edge[j][i] = 1;
                }
                rec[site].push_back(i);
            }
        }
        queue<int> q;
        for(int s : rec[source]){
            q.push(s);
            uset[s] = 1;
        }
        while(!q.empty()){
            int route = q.front();
            q.pop();
            for(int i = 0; i < n; ++i){
                if(edge[route][i] == 1 && uset[i] == 0){
                    uset[i] = uset[route] + 1;
                    q.push(i);
                }
            }
        }
        int ans = INT_MAX;
        for(int s : rec[target]){
            if(uset[s] != 0) ans = min(ans, uset[s]);
        }
        return ans == INT_MAX ? -1 : ans;
    }
};
// @lc code=end

