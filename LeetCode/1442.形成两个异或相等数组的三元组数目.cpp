/*
 * @lc app=leetcode.cn id=1442 lang=cpp
 *
 * [1442] 形成两个异或相等数组的三元组数目
 */

// @lc code=start
class Solution {
public:
    int countTriplets(vector<int>& arr) {
        int n = arr.size();
        vector<int> xor_vec(n + 1, 0);
        for(int i = 0; i < n; ++i){
            xor_vec[i + 1] = xor_vec[i] ^ arr[i];
        }
        int ans = 0;
        for(int i = 0; i < n; ++i){
            for(int j = i + 1; j < n; ++j){
                if((xor_vec[j + 1] ^ xor_vec[i]) == 0){
                    ans += j - i;
                }
            }
        }
        return ans;
    }
};
// @lc code=end

