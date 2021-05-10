/*
 * @lc app=leetcode.cn id=496 lang=cpp
 *
 * [496] 下一个更大元素 I
 */

// @lc code=start
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> m;
        stack<int> s;
        int len1 = nums1.size(), len2 = nums2.size();
        for(int i = 0; i < len2; ++i){
            while(!s.empty() && s.top() < nums2[i]){
                m[s.top()] = nums2[i];
                s.pop();
            }
            s.push(nums2[i]);
        }
        while(!s.empty()){
            m[s.top()] = -1;
            s.pop();
        }
        vector<int> res(len1, 0);
        for(int i = 0; i < len1; ++i){
            res[i] = m[nums1[i]];
        }
        return res;
    }
};
// @lc code=end

