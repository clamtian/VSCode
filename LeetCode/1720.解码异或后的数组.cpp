/*
 * @lc app=leetcode.cn id=1720 lang=cpp
 *
 * [1720] 解码异或后的数组
 */

// @lc code=start
class Solution {
public:
    vector<int> decode(vector<int>& encoded, int first) {
        int n = encoded.size();
        vector<int> res(n + 1, 0);
        res[0] = first;
        for(int i = 1; i <= n; ++i){
            res[i] = res[i - 1] ^ encoded[i - 1];
        }
        return res;
    }
};
// @lc code=end

