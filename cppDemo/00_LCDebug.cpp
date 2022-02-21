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
#define lowbit(x) ((x) & (-x))
class Solution {
public:
    long long goodTriplets(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        unordered_map<int, int> umap;
        for(int i = 0; i < n; ++i) umap[nums2[i]] = i;
        for(int i = 0; i < n; ++i){
            nums2[umap[nums1[i]]] = i;
                  nums1[i] = i;
        }
        long long ans = 0;
        vector<long long> tree(n + 1, 0);
        for(int i = 0; i < n; ++i){
            long long x = query(nums2[i] + 1, tree);
            ans += x * (n - i - 1 - i + x);
            update(nums2[i] + 1, 1, tree);
        }
        return ans;
    }
    void update(int i, int x, vector<long long>& tree){
        for (int pos = i; pos < tree.size(); pos += lowbit(pos))
            tree[pos] += x;
    }
    int query(int n, vector<long long>& tree){
        int ans = 0;
        for (int pos = n; pos; pos -= lowbit(pos))
            ans += tree[pos];
        return ans;
    }
};

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
    vector<int> b = { 2,0,1,3 };
    vector<char> vc = { 'A', 'A', 'A', 'B', 'B', 'B' };
    vector<int> b_ = { 0,1,2,3 };

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
    so.goodTriplets(b, b_);
    return 0;
}
