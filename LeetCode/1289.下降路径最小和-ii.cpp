/*
 * @lc app=leetcode.cn id=1289 lang=cpp
 *
 * [1289] 下降路径最小和  II
 */

// @lc code=start
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& arr) {
        int m = arr.size(), n = arr[0].size();
        vector<int> dp1 = arr[0], dp2(n, 0);
        int kmin = 0, knext = 0, minval = INT_MAX, secondval = INT_MAX;
        for(int i = 0; i < n; ++i){
            if(arr[0][i] < minval){
                secondval = minval;
                minval = arr[0][i];
                knext = kmin;
                kmin = i;
            }else if(arr[0][i] < secondval){
                knext = i;
                secondval = arr[0][i];
            }
        }
        for(int i = 1; i < m; ++i){
            for(int j = 0; j < n; ++j){
                if(j != kmin){
                    dp2[j] = arr[i][j] + dp1[kmin];
                }else{
                    dp2[j] = arr[i][j] + dp1[knext];
                }
            }
            kmin = 0, knext = 0, minval = INT_MAX;
            for(int j = 0; j < n; ++j){
                if(dp2[j] < minval){
                    secondval = minval;
                    minval = dp2[j];
                    knext = kmin;
                    kmin = j;
                }else if(dp2[j] < secondval){
                    knext = j;
                    secondval = dp2[j];
                }
            }
            swap(dp1, dp2);
        }
        return *min_element(dp1.begin(), dp1.end());
    }
};
// @lc code=end

