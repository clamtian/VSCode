#include <vector>
#include <string>
#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/**
 * 实现二叉树的先序 中序 后序遍历
 * 包括递归和非递归方式
 */

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void preOrderRecur(TreeNode* head) {
	if(!head) return;
    cout << head->val << " " << endl;
	preOrderRecur(head->left);
	preOrderRecur(head->right);
}

void preOrderUnRecur(TreeNode* head) {
    if(!head) return;
    TreeNode* node = head;
    stack<TreeNode*> s;
    s.push(node);
    while(s.size()){
        node = s.top();
        s.pop();
        cout << node->val << " " << endl;
        if(node->right) s.push(node->right);
        if(node->left) s.push(node->left);
    }
}

void inOrderRecur(TreeNode* head) {
	if (!head) return;
	inOrderRecur(head->left);
	cout << head->val << " " << endl;
	inOrderRecur(head->right);
}

void inOrderUnRecur(TreeNode* head) {
    if (!head) return;
    stack<TreeNode*> s;
    TreeNode *node = head;
    while (s.size() || node) {
        if (node) {
            s.push(node);
            node = node->left;
        } else {
            node = s.top();
            s.pop();
            cout << node->val << " " << endl;
            node = node->right;
        }
    }
}

void posOrderRecur(TreeNode* head) {
	if(!head) return;
	posOrderRecur(head->left);
	posOrderRecur(head->right);
	cout << head->val << " " << endl;
}


void posOrderUnRecur1(TreeNode* head) {
    if(!head) return;
	stack<TreeNode*> s1, s2;
	s1.push(head);
	while (s1.size()) {
		head = s1.top();
        s1.pop();
		s2.push(head);
		if (head->left) s1.push(head->left);
		if (head->right) s1.push(head->right);
		while (s2.size()) {
            cout << s2.top()->val << " " << endl;
            s2.pop();
        }
    }
}

void posOrderUnRecur2(TreeNode* head) {
    if(!head) return;
	stack<TreeNode*> s;
	s.push(head);
	TreeNode* node, *h = head;
	while (s.size()) {
		node = s.top();
		if (node->left && h != node->left && h != node->right){
            s.push(node->left);
        } else if (node->right && h != node->right) {
            s.push(node->right);
        }else {
            cout << s.top()->val << " " << endl;
            s.pop();
            h = node;
		}
	}
}

int main(){

    return 1;
}