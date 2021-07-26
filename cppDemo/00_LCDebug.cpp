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
const int N = 1e5;
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
    int minOperations(vector<int>& target, vector<int>& arr) {
        int n = target.size(), m = arr.size();
        unordered_map<int, int> umap;
        for(int i = 0; i < n; ++i) umap[target[i]] = i;
        for(int i = 0; i < m; ++i){
            if(umap.find(arr[i]) != umap.end()) arr[i] = umap[arr[i]];
            else arr[i] = -1;
        }
        vector<int> low = { arr[0] };
        for(int i = 0; i < m; ++i){
            if(arr[i] > low.back()) low.push_back(arr[i]);
            else{
                int l = 0, r = low.size();
                while(l < r) {
                    int mid = (l + r) >> 1;
                    if(low[mid] < arr[i]) l = mid + 1;
                    else r = mid;
                }
                low[l] = arr[i];
            }
        }
        if(low[0] != -1) return n - low.size();
        return n - low.size() + 1;
    }
};



int main() {

    
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


    vector<vector<int>> a = {{2,9,10},{3,7,15},{5,12,12},{15,20,10},{19,24,8}};
    vector<int> b = { 16,7,20,11,15,13,10,14,6,8 };
    vector<int> b_ = { 11,14,15,7,5,5,6,10,11,6 };

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
    so.minOperations(b, b_);
    return 0;
}
