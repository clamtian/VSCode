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



class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.size(), n = text2.size();
        vector<pair<int, int>> dp(n); //<length, pos>
        for(int j = 0; j < m; ++j){
            if(text1[j] == text2[0]){
                dp[0].first = 1;
                dp[0].second = j;
                break;
            }
        }
        for(int i = 1; i < n; ++i){
            int maxSize = 0, maxIndex = -1;
            for(int k = 0; k < i; ++k){
                int pos = dp[k].second + 1;
                while(pos < m){
                    if(text1[pos] == text2[i] && maxSize < dp[k].second){
                        maxSize = dp[k].second;
                        maxIndex = pos;
                        break;
                    }
                    ++pos;
                }
            }
            dp[i].first = maxSize + 1;
            dp[i].second = maxIndex;
        }
        int ans = 0;
        for(int i = 0; i < n; ++i){
            ans = max(ans, dp[i].first);
        }
        return ans;
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {3,4},{1,100},{2,2},{5,5} };
    vector<int> b = { 2,7,9,4,4 };
    vector<int> b_ = { 9,3,5,1,7,4 };

    list<int> li(b.begin(), b.end());

    b.erase(remove(b.begin(), b.end(), 1), b.end());
    li.remove(1);



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
    vector<string> str = { "leet", "code" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    string st = ", 234 ;";
    cout << atoi(st.c_str()) << endl;
    //cout << so.func(4, a) << endl;
    so.longestCommonSubsequence("abcde", "ace");
}
