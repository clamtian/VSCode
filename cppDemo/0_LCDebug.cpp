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
typedef unsigned long long ULL;
const int N = 100010;
ULL h[N], p[N]; // h[k]存储字符串前k个字母的哈希值, p[k]存储 P^k mod 2^64
int P = 133331;


// 计算子串 str[l ~ r] 的哈希值
ULL get(int l, int r){
    return h[r] - h[l - 1] * p[r - l + 1];
}

class Solution {
public:
    unordered_map<ULL, int> umap;
    unordered_set<ULL> s;
    bool check(int l, vector<vector<int>>& paths){
        umap.clear();
        int m = paths.size();
        for(int i = 0; i < m; ++i){
            s.clear();
            int n = paths[i].size();
            for (int k = 1; k <= n; ++k){
                h[k] = h[k - 1] * P + paths[i][k - 1];
                p[k] = p[k - 1] * P;
            }
            for(int j = l; j <= n; ++j){
                s.insert(get(j - l + 1, j));
            }
            for(auto path : s) umap[path]++;
        }
        for(auto& [path, num] : umap){
            if(num == m) return true;
        }
        return false;
    }
    int longestCommonSubpath(int n, vector<vector<int>>& paths) {
        int l = 0, r = N;
        p[0] = 1;
        for(auto& path : paths) r = min(r, (int)path.size());
        while(l < r){
            int mid = l + (r - l + 1) >> 1;
            if(check(mid, paths)) l = mid;
            else r = mid - 1;
        }
        return l;
    }
};


int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = {{0,1,2,3,4},{2,3,4},{4,0,1,2,3}};
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
    so.longestCommonSubpath(5, a);

    return 0;
}
