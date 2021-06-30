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
class Solution {
public:
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
        vec.push_back(data.substr(pos));
        pos = 0, n = vec.size();
        TreeNode* root = new TreeNode(stoi(vec[pos++]));
        s1.push(root);
        while(!s1.empty() && pos < n){
            while(!s1.empty()){
                TreeNode* left, * right;
                if(vec[pos] != "null") left = new TreeNode(stoi(vec[pos]));
                ++pos;
                if(vec[pos] != "null") right = new TreeNode(stoi(vec[pos]));
                ++pos;
                TreeNode* node = s1.top();
                s1.pop();
                if(node){
                    node->left = left;
                    node->right = right;
                }
                s2.push(left);
                s2.push(right);
            }
            while(!s2.empty()){
                s1.push(s2.top());
                s2.pop();
            }
        }
        return root;
    }
};


int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {12}, {9,11},{5,7,10,14}};
    vector<int> b = { 3,5,8,8,8,10,11,12 };
    vector<int> b_ = { 45,57,38,64,52,92,31,57,31,52,3,12,93,8,11,60,55,92,42,27,40,10,77,53,8,34,87,39,8,35,28,70,32,97,88,54,82,54,54,10,78,23,82,52,10,49,8,36,9,52,81,26,5,2,30,39,89,62,39,100,67,33,86,22,49,15,94,59,47,41,45,17,99,87,77,48,22,77,82,85,97,66,3,38,49,60,66 };

    vector<vector<char>> ch_vec = { {'1', '1', '1'},
                               {'0', '1', '0'},
                               {'1', '1', '1'} };
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

    TreeNode* r1 = new TreeNode(1);
    TreeNode* r2 = new TreeNode(2);
    TreeNode* r3 = new TreeNode(3);
    TreeNode* r4 = new TreeNode(4);
    TreeNode* r5 = new TreeNode(5);

    r1->left = r2;
    r1->right = r3;
    r3->left = r4;
    r3->right = r5;


    string t = "alex";
    string s = "ababaab";
    char ch = 49 + '0';
    vector<bool> is(2, true);
    vector<string> str = { "E23", "2X2", "12S" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    so.deserialize("1,2,3,null,null,4,5");

    return 0;
}
