#include <string.h>
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
    int dp[2][29][29][29];
    vector<int> earliestAndLatest(int n, int a, int b) {
        vector<int> res = { 0, 0 };
        res[0] = dfs(n, a, b, 0);
        res[1] = dfs(n, a, b, 1);
        return res;
    }
    int dfs(int n, int a, int b, int flag){
        if(dp[flag][n][a][b] != 0) return dp[flag][n][a][b];
        if(a > b) swap(a, b);
        int tot = (n + 1) / 2, mid = n / 2 + 1;;
        int ans = 0;
        if(flag == 0) ans = INT_MAX;
        if(a < mid && (b < mid || b == mid && n % 2 == 1)){
            for(int i = 0; i < a; ++i){
                for(int j = 0; j < b - a; ++j){
                    if(flag == 0) ans = min(ans, dfs(tot, i + 1, i + j + 2, 0));
                    else ans = max(ans, dfs(tot, i + 1, i + j + 2, 1));
                }
            }
            ans += 1;
        }else if(a >= mid && b > mid){
            if(flag == 0) ans = dfs(n, n + 1 - b, n + 1 - a, 0);
            else ans = dfs(n, n + 1 - b, n + 1 - a, 1);
        }else if(a < mid && b >= mid){
            if(b == n + 1 - a){
                ans = 1;
            }else if(b > n + 1 - a){
                if(flag == 0) ans = dfs(n, n + 1 - b, n + 1 - a, 0);
                else ans = dfs(n, n + 1 - b, n + 1 - a, 1);
            }else{
                for(int i = 0; i < a; ++i){
                    for(int j = 0; j < n + 1 - b - a; ++j){
                        if(flag == 0) ans = min(tot, dfs(mid, i + 1, i + j + ((n - 2 * (n + 1 - b) + 1) / 2 + 2), 0));
                        else ans = max(ans, dfs(tot, i + 1, i + j + ((n - 2 * (n + 1 - b) + 1) / 2 + 2), 1));
                    }
                }
                ans += 1;
            }
        }
        dp[flag][n][a][b] = ans;
        return dp[flag][n][a][b];
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

    string t = "alex";
    string s = "cegfxvulsxakw";
    char ch = 49 + '0';
    vector<bool> is(2, true);
    vector<string> str = { "i", "love", "leetcode", "i", "love", "coding" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    string st = ", 234 ;";
    string s1 = "011010";
    string s2 = "love";
    if(s1 < s2) cout << 666 << endl;
    cout << atoi(st.c_str()) << endl;
    //cout << so.func(4, a) << endl;
    double ss = 2.01;
    so.earliestAndLatest(4,1,3);

    return 0;
}
