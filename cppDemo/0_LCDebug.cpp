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
    unordered_map<int, vector<int>> umap;
    unordered_set<int> uset;
    int largestPathValue(string colors, vector<vector<int>>& edges) {
        for(auto& edge : edges) umap[edge[0]].push_back(edge[1]);
        int n = colors.size(), ans = 0;
        vector<vector<int>> dp(n, vector<int>(27, 0));
        for(int i = 0; i < n; ++i){
            auto vec = dfs(i, colors, dp);
            uset.clear();
            if(vec[0] == -1) return -1;
            else ans = *max_element(vec.begin() + 1, vec.end());
        }
        return ans;
    }
    vector<int> dfs(int k, string& colors, vector<vector<int>>& dp){
        if(dp[k][0] != 0) return dp[k];
        if(uset.find(k) != uset.end()){
            dp[k][0] = -1;
            return dp[k];
        }
        uset.insert(k);
        for(int node : umap[k]){
            vector<int> vec = dfs(node, colors, dp);
            if(vec[0] == -1){
                dp[k][0] = -1;
                return dp[k];
            }
            for(int i = 1; i < 27; ++i){
                dp[k][i] = max(dp[k][i], vec[i]);
            }
        }
        uset.erase(k);
        dp[k][colors[k] - 'a' + 1]++;
        dp[k][0] = 1;
        return dp[k];
    }
};


int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {0, 1}, {0, 2},{2, 3}, {3, 4}};
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
    so.largestPathValue("abaca", a);

    return 0;
}
