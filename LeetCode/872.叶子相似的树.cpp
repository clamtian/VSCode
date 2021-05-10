/*
 * @lc app=leetcode.cn id=872 lang=cpp
 *
 * [872] 叶子相似的树
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
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        stack<TreeNode*> s;
        list<int> ls;
        TreeNode* node = root1;
        while(!s.empty() || node){
            if(node){
                s.push(node);
                if(!node->left && !node->right) ls.push_back(node->val);
                node = node->left;
            }else{
                node = s.top();
                s.pop();
                node = node->right;
            }
        }
        node = root2;
        while(!s.empty() || node){
            if(node){
                s.push(node);
                if(!node->left && !node->right) {
                    if(!ls.empty() && node->val == ls.front())
                        ls.pop_front();
                    else return false;
                }
                node = node->left;
            }else{
                node = s.top();
                s.pop();
                node = node->right;
            }
        }
        return ls.empty();
    }
};
// @lc code=end

