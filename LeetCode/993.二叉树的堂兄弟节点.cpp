/*
 * @lc app=leetcode.cn id=993 lang=cpp
 *
 * [993] 二叉树的堂兄弟节点
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
    TreeNode* x_father = nullptr;
    TreeNode* y_father = nullptr;
    int g_x, g_y;
    bool isCousins(TreeNode* root, int x, int y) {
        if(root->val == x || root->val == y) return false;
        g_x = x;
        g_y = y;
        int x_depth = find(root, x, 0);
        int y_depth = find(root, y, 0);
        return x_depth == y_depth && !(x_father == y_father);
    }
    int find(TreeNode* root, int val, int depth){
        if(!root) return 0;
        if(root->left){
            if(root->left->val == val){
                if(val == g_x) x_father = root;
                else y_father = root;
                return depth;
            }else{
                int d = find(root->left, val, depth + 1);
                if(d != 0) return d;
            }
        }
        if(root->right){
            if(root->right->val == val){
                if(val == g_x) x_father = root;
                else y_father = root;
                return depth;
            }else{
                int d = find(root->right, val, depth + 1);
                if(d != 0) return d;
            }
        }
        return 0;
    }
};
// @lc code=end

