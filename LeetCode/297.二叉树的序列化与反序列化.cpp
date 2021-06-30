// @before-stub-for-debug-begin
#include <vector>
#include <stack>
#include <string>
#include "commoncppproblem297.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=297 lang=cpp
 *
 * [297] 二叉树的序列化与反序列化
 */

// @lc code=start

 //Definition for a binary tree node.
 struct TreeNode {
     int val;
     TreeNode *left;
     TreeNode *right;
     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };
 
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        stack<TreeNode*> s1, s2;
        string res = "";
        if(!root) return "null";
        int flag = 1;
        s1.push(root);
        while(!s1.empty() && flag){
            flag = 0;
            while(!s1.empty()){
                auto node = s1.top();
                s1.pop();
                if(!node) res += "null,";
                else{
                    flag = 1;
                    res += to_string(node->val) + ",";
                    s2.push(node->left);
                    s2.push(node->right);
                }
            }
            swap(s1, s2);
            s2 = stack<TreeNode*>();
        }
        res.pop_back();
        cout << "serialize end" << endl;
        return res;
    }


    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        stack<TreeNode*> s1, s2;
        if(data == "null") return nullptr;
        vector<string> vec;
        int pos = 0, n = data.size();
        while(pos < n && data.find(',', pos) != string::npos){
            int pos_ = data.find(',', pos);
            vec.push_back(data.substr(pos, pos_ - pos));
            pos = pos_ + 1;
        }
        pos = 0, n = vec.size();
        TreeNode* root = new TreeNode(stoi(vec[pos++]));
        s1.push(root);
        while(!s1.empty() && pos < n){
            while(!s1.empty()){
                TreeNode* left, * right;
                if(vec[pos] != "null") left = new TreeNode(stoi(vec[pos++]));
                if(vec[pos] != "null") right = new TreeNode(stoi(vec[pos++]));
                TreeNode* node = s1.top();
                s1.pop();
                if(node){
                    node->left = left;
                    node->right = right;
                }
                s2.push(left);
                s2.push(right);
            }
            swap(s1, s2);
            s2 = stack<TreeNode*>();
        }
        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
// @lc code=end

