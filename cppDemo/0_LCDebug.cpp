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
//int a[] = { 1, 2, 2, 3, 1, 4, 2 };

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
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        int m = group.size();
        int tot = accumulate(profit.begin(), profit.end(), 0);
        int dp[n + 1][tot + 1][m + 1];
        for(int i = 0; i <= n; ++i){
            for(int j = 0; j <= tot; ++j){
                for(int k = 0; k <= m; ++k){
                    cout << i << endl;
                        cout << j << endl;
                        cout << k << endl;
                    dp[i][j][k] = j == 0 ? 1 : 0;
                }
            }
        }
        for(int i = 1; i <= n; ++i){
            for(int j = 1; j <= tot; ++j){
                for(int k = 1; k <= m; ++k){
                    dp[i][j][k] = dp[i][j][k - 1];
                    if(i >= group[k - 1] && j >= profit[k - 1]){
                        dp[i][j][k] += dp[i - group[k - 1]][j - profit[k - 1]][k - 1];
                    }
                }
            }
        }
        int ans = 0;
        for(int j = minProfit; j <= tot; ++j){
            ans += dp[n][j][m];
        }
        return ans;
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {12,4}, {8,1}, {6,3}};
    vector<int> b = { 82,7,18,34,1,3,83,56,50,34,39,38,76,92,71,2,6,74,1,82,22,73,88,98,6,71,6,26,100,75,57,88,43,16,22,89,7,9,78,97,22,87,34,81,74,56,49,94,87,71,59,6,20,66,64,37,2,42,30,87,73,16,39,87,28,9,95,78,43,59,87,78,2,93,7,22,21,59,68,67,65,63,78,20,82,35,86 };
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
    int x = so.profitableSchemes(95,53,b,b_);

    return 0;
}
