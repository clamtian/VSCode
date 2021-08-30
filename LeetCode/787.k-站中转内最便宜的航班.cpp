// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem787.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=787 lang=cpp
 *
 * [787] K 站中转内最便宜的航班
 */

// @lc code=start
typedef pair<int, int> PII;
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<int> dist(n, INT_MAX / 2), back(n, INT_MAX / 2), st(n, 0);
        dist[src] = 0;
        st[src] = 1;
        vector<vector<PII>> g(n);
        for(auto f : flights) g[f[0]].push_back({ f[1], f[2] });
        queue<int> q;
        q.push(0);
        while(!q.empty() && k-- >= 0){
            queue<int> q1;
            back = dist;
            while(!q.empty()){
                int a = q.front();
                q.pop();
                st[a] = 0;
                for(auto [b, w] : g[a]){
                    if(dist[b] > back[a] + w){
                        dist[b] = back[a] + w;
                        if(!st[b]){
                            q1.push(b);
                            st[b] = 1;
                        }
                    }
                }
            }
            q = q1;
        }
        return dist[dst] == INT_MAX / 2 ? -1 : dist[dst];
    }
};
// @lc code=end

