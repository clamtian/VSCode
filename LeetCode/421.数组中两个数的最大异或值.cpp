// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem421.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=421 lang=cpp
 *
 * [421] 数组中两个数的最大异或值
 */

// @lc code=start
class Tree { 
public:
    int val;
    Tree* left, * right;
    Tree(int num) : val(num), left(nullptr), right(nullptr) {}
};
class Solution {

public:
    int findMaximumXOR(vector<int>& nums) {
        Tree* root = new Tree(0);
        int ans = 0, n = nums.size();
        for(int i = 1; i < n; ++i){
            add(nums[i - 1], root);
            ans = max(ans, search(nums[i], root));
        }
        return ans;
    }
    int search(int num, Tree* root){
        int ans = 0;
        Tree* node = root;
        for(int k = 30; k >= 0; --k){
            if((num >> k) & 1){
                if(node->left){
                    ans = ans * 2 + 1;
                    node = node->left;
                }else{
                    ans *= 2;
                    node = node->right;
                }
            }else{
                if(node->right){
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
    void add(int num, Tree* root){
        Tree* node = root;
        for(int k = 30; k >= 0; --k){
            if((num >> k) & 1){
                if(!node->right) node->right = new Tree(1);
                node = node->right;
            }else{
                if(!node->left) {
                    node->left = new Tree(0);
                }
                node = node->left;
            }
        }
    }
};
// @lc code=end

