/*
 * @lc app=leetcode.cn id=1734 lang=cpp
 *
 * [1734] 解码异或后的排列
 */

// @lc code=start
class Solution {
public:
    vector<int> decode(vector<int>& encoded) {
        int n = encoded.size() + 1;
        int k = 0;
        for(int i = 1; i <= n; ++i) k ^= i;
        for(int i = 0; i < n - 1; i += 2){
            k ^= encoded[i];
        }
        vector<int> res(n, 0);
        res[n - 1] = k;
        for(int i = n - 2; i >= 0; --i){
            res[i] = res[i + 1] ^ encoded[i];
        }
        return res;
    }
};
// @lc code=end

