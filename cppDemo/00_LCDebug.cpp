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
    int MOD = 1e9 + 7;
    int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
        vector<vector<int>> cop;
        int n = nums1.size(), ans = 0;
        for(int i = 0; i < n; ++i) cop.push_back({ nums2[i], nums1[i] });
        sort(cop.begin(), cop.end());
        for(int i = 0; i < n; ++i){
            int l = 0, r = n - 1;
            while(l < r){
                int mid = l + r >> 1;
                if(cop[mid][0] > nums1[i]) r = mid;
                else l = mid + 1;
            }
            int cmp = 0;
            cmp = min(cmp, abs(nums1[i] - cop[l][0]) - abs(cop[l][1] - cop[l][0]));
            if(l != n - 1) cmp = min(cmp, abs(nums1[i] - cop[l + 1][0]) - abs(cop[l + 1][1] - cop[l + 1][0]));
            if(l != 0) cmp = min(cmp, abs(nums1[i] - cop[l - 1][0]) - abs(cop[l - 1][1] - cop[l - 1][0]));
            ans = min(ans, cmp);
        }
        for(int i = 0; i < n; ++i) ans = ans % MOD + abs(nums1[i] - nums2[i]);
        return ans;
    }
};



int main() {

    vector<vector<int>> a = {{2,9,10},{3,7,15},{5,12,12},{15,20,10},{19,24,8}};
    vector<int> b = { 1,7,5 };
    vector<int> b_ = { 2,3,5 };

    vector<vector<char>> ch_vec = { {'1', '1', '1'},
                               {'0', '1', '0'},
                               {'1', '1', '1'} };
    vector<vector<string>> svec = { {"David","3","Ceviche"},
                                {"Corina","10","Beef Burrito"},
                                {"David","3","Fried Chicken"},
                                {"Carla","5","Water"},
                                {"Carla","5","Ceviche"},
                                {"Rous","3","Ceviche"} };
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
    so.minAbsoluteSumDiff(b, b_);
    return 0;
}
