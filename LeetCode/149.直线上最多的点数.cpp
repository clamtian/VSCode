/*
 * @lc app=leetcode.cn id=149 lang=cpp
 *
 * [149] 直线上最多的点数
 */

// @lc code=start
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if(n < 3) return n;
        int ans = 2, cmp = 2;
        vector<int> is_included(n, 0);
        for(int i = 0; i < n; ++i){
            for(int j = i + 1; j < n; ++j){
                if(is_included[j] == 1) continue;
                for(int k = j + 1; k < n; ++k){
                    if(isline(points[i], points[j], points[k])){
                        ++cmp;
                        is_included[k] = 1;
                    }
                }
                ans = max(ans, cmp);
                cmp = 2;
            }
            for(int j = 0; j < n; ++j) is_included[j] = 0;
        }
        return ans;
    }
    bool isline(vector<int>& a, vector<int>& b, vector<int>& c){
        int x1 = b[0] - a[0], x2 = c[0] - b[0];
        int y1 = b[1] - a[1], y2 = c[1] - b[1];
        return x1 * y2 == x2 * y1;

    }
};
// @lc code=end

