// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <stack>
#include "commoncppproblem897.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=897 lang=cpp
 *
 * [897] 递增顺序查找树
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
    TreeNode* increasingBST(TreeNode* root) {
        stack<TreeNode *> s;
        TreeNode *node = root, *n = NULL;
        int flag = 0;
        while(!s.empty() || node){
            if(node){
                s.push(node);
                node = node->left;
            }else{
                if(!flag){
                    root = s.top();
                    n = root;
                    flag = 1;
                }else{
                    n->right = s.top();
                    n = n->right;
                }
                node = s.top()->right;
                s.top()->left = NULL;
                s.pop();
            }
        }
        return root;
    }
};
// @lc code=end

