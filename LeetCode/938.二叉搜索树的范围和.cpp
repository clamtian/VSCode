/*
 * @lc app=leetcode.cn id=938 lang=cpp
 *
 * [938] 二叉搜索树的范围和
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        int sum = 0;
        stack<TreeNode *> s;
        TreeNode *node = root;
        int flag = 0;
        while(s.size() != 0 || node){
            if(node){
                s.push(node);
                node = node->left;
            }else{
                int num = s.top()->val;
                if(flag){
                    sum += num;
                    if(num == high) return sum;
                }else if(num == low){
                    flag = 1;
                    sum += num;
                }
                node = s.top()->right;
                s.pop();
            }
        }
        return sum;
    }
};
// @lc code=end

