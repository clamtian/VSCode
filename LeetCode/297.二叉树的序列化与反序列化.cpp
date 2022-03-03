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
 
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string res;
        if(!root) return res;
        TreeNode* node = root;
        stack<TreeNode*> s;
        s.push(node);
        TreeNode* z = new TreeNode(0);
        while(s.size()){
            node = s.top();
            s.pop();
            if(node == z) res += "_#";
            else{
                res += "#" + to_string(node->val);
                if(node->right) s.push(node->right);
                if(node->left) s.push(node->left);
            }
        }
        return res;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        vector<string> v = preProcess(data);
        int n = v.size();
        if(!n) return NULL;
        TreeNode* head = new TreeNode(stoi(v[0]));
        TreeNode* node = head;
        stack<TreeNode*> s;
        s.push(head);
        int idx = 1;
        while(idx < n){
            node = s.top();
            s.pop();
            cout << v[idx] << endl;
            if(v[idx] != "#"){
                TreeNode* l = new TreeNode(stoi(v[idx]));
                node->left = l;
                s.push(l);
            }else if(idx + 1 < n && v[++idx] != "#"){
                TreeNode* r = new TreeNode(stoi(v[idx]));
                node->right = r;
                s.push(r);
            }
            ++idx;
        }
        return head;
    }
    vector<string> preProcess(string& s){
        int n = s.size();
        int p = 0;
        vector<string> res;
        while(p < n){
            int t = p;
            while(t < p && s[t] != '_') ++t;
            string str = s.substr(p, t - p);
            res.push_back(str);
            p = t + 1;
        }
        return res;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));
// @lc code=end

