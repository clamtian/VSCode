// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <stack>
#include "commoncppproblem105.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=105 lang=cpp
 *
 * [105] 从前序与中序遍历序列构造二叉树
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
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        stack<TreeNode *> s;
        int n = preorder.size();
        if (!n) return NULL;
        TreeNode *head = NULL;
        int i = 0, j = 0;
        while (i < n) {
            if (!s.size() || s.top()->val != inorder[j]) {
                TreeNode *node = new TreeNode(preorder[i++]);
                if (!head) head = node;
                if (s.size()) s.top()->left = node;
                s.push(node);
            } else {
                TreeNode *h = NULL;
                while (s.size() && j < n && s.top()->val == inorder[j]) {
                    h = s.top();
                    s.pop();
                    ++j;
                }
                if (j < n && head) {
                    TreeNode *right = new TreeNode(preorder[i]);
                    h->right = right;
                }
            }
        }
        return head;
    }
};
// @lc code=end

