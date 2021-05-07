/*
 * @lc app=leetcode.cn id=1819 lang=cpp
 *
 * [1819] 序列中不同最大公约数的数目
 */

// @lc code=start
class Solution {
public:
    int countDifferentSubsequenceGCDs(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        int maxVal = *max_element(nums.begin(), nums.end());
        vector<int> isExist(maxVal + 1, 0);
        for(int num : nums) isExist[num] = 1;
        for(int gcd = 1; gcd <= maxVal; ++gcd){
            if(isMaxGcd(gcd, isExist, maxVal)){
                ++ans;
            }
        }
        return ans;
    }
    bool isMaxGcd(int gcd, vector<int>& isExist, int maxVal){
        if(isExist[gcd]) return true;
        int tmp = 0;
        for(int nGcd = gcd; nGcd <= maxVal; nGcd += gcd){
            if(!isExist[nGcd]) continue;
            if(tmp == 0) tmp = nGcd;
            tmp = __gcd(tmp, nGcd);
            if(tmp == gcd) return true;
        }
        return false;
    }
};
// @lc code=end

