#include <string.h>
#include <math.h>
#include <vector>
#include <string>

#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 


using namespace std;

typedef long long LL;
int MOD = 1e9 + 7;
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
    
};
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
    
};

bool compare(const pair<string, int>& p1, const pair<string, int>& p2){
    if(p1.second == p2.second){
        return p1.first > p2.first;
    }
    return p1.second > p2.second;
}

typedef pair<int, int> PII;
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
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
            if(node == z) res += "#_";
            else{
                res += to_string(node->val) + "_";
                if(node->right) s.push(node->right);
                else s.push(z);
                if(node->left) s.push(node->left);
                else s.push(z);
            }
        }
        res.pop_back();
        cout << res << endl;
        return res;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        vector<string> v = preProcess(data);
        int n = v.size();
        if(!n) return NULL;
        TreeNode* head = new TreeNode(stoi(v[0]));
        int p = 1;
        dfs(head, v, p);
        return head;
    }
    void dfs(TreeNode* head, vector<string>& v, int& p){
        if(p < v.size() && v[p] != "#"){
            TreeNode* l = new TreeNode(stoi(v[p]));
            head->left = l;
            dfs(head->left, v, ++p);
        }
        ++p;
        if(p < v.size() && v[p] != "#"){
            TreeNode* r = new TreeNode(stoi(v[p]));
            head->right = r;
            dfs(head->right, v, ++p);
        }
    } 
    vector<string> preProcess(string& s){
        int n = s.size();
        int p = 0;
        vector<string> res;
        while(p < n){
            int t = p;
            while(t < n && s[t] != '_') ++t;
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

int main() {

    
    ///cout << search("bad") << endl;
    ListNode x1(5);
    ListNode x2(2);
    ListNode x3(3);
    ListNode x4(4);
    ListNode x5(1);
    x5.next = &x4;
    x4.next = &x3;
    x3.next = &x2;
    x2.next = &x1;
    ListNode* head = &x5;

    TreeNode* r0 = new TreeNode(0);
    TreeNode* r1 = new TreeNode(1);
    TreeNode* r2 = new TreeNode(2);
    TreeNode* r3 = new TreeNode(3);
    TreeNode* r4 = new TreeNode(4);
    TreeNode* r5 = new TreeNode(5);

    r0->left = r1;
    r1->right = r2;
    r0->right = r3;
    //r3->left = r4;
    //r3->right = r5;


    string t = "alex";
    string s = "ababaab";
    char ch = 49 + '0';
    vector<bool> is(2, true);
    vector<string> str = { "E23", "2X2", "12S" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };


    vector<vector<int>> a = {{1,0,0,0},{1,0,0,0}, { 1,0,0,0}, {1,0,0,0}};
    vector<int> b = { 2,3,4,0,5,1 };
    vector<int> b_ = { 5,2,4,3,1,0 };
    vector<char> vc = { 'A', 'A', 'A', 'B', 'B', 'B' };
    

    vector<vector<char>> ch_vec = { {'1', '1', '1'},
                               {'0', '1', '0'},
                               {'1', '1', '1'} };
    vector<vector<string>> svec = { {"David","3","Ceviche"},
                                {"Corina","10","Beef Burrito"},
                                {"David","3","Fried Chicken"},
                                {"Carla","5","Water"},
                                {"Carla","5","Ceviche"},
                                {"Rous","3","Ceviche"} };
    Solution so;
    so.deserialize("1_2_#_#_3_4_#_#_5_#_#");
    return 0;
}
