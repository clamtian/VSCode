/*
 * @lc app=leetcode.cn id=1707 lang=cpp
 *
 * [1707] 与数组中元素的最大异或值
 */

// @lc code=start

class Solution {
public:
    struct TreeNode{
        int val;
        TreeNode* left; //for 0
        TreeNode* right; //for 1
        TreeNode(int num) : val(num), left(nullptr), right(nullptr) { }
    };
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        int n = queries.size(), m = nums.size();
        vector<int> res(n, 0);
        
        TreeNode* root = new TreeNode(INT_MAX);

        for(int i = 0; i < m; ++i){
            root->val = min(root->val, nums[i]);
            add(nums[i], root);
        }
        for(int i = 0; i < n; ++i){
            res[i] = search(queries[i][0], queries[i][1], root);
        }
        return res;
    }
    int search(int num, int abort, TreeNode* root){
        if(root->val > abort) return -1;
        int ans = 0;
        TreeNode* node = root;
        for(int k = 30; k >= 0; --k){
            if((num >> k) & 1){
                if(node->left && node->left->val <= abort){
                    ans = ans * 2 + 1;
                    node = node->left;
                }else{
                    ans *= 2;
                    node = node->right;
                }
            }else{
                if(node->right && node->right->val <= abort){
                    ans = ans * 2 + 1;
                    node = node->right;
                }else{
                    ans *= 2;
                    node = node->left;
                }
            }
        }
        return ans;
    }
    void add(int num, TreeNode* root){
        TreeNode* node = root;
        for(int k = 30; k >= 0; --k){
            if((num >> k) & 1){
                if(!node->right){
                    node->right = new TreeNode(num);
                }else{
                    node->right->val = min(num, node->right->val);
                }
                node = node->right;
            }else{
                if(!node->left) {
                    node->left = new TreeNode(num);
                }else{
                    node->left->val = min(node->left->val, num);
                }
                node = node->left;
            }
        }
    }
};
// @lc code=end

