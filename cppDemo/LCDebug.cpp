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
    int stoneGameVIII(vector<int>& stones) {
        int n = stones.size();
        vector<int> dp(n + 1, 0);
        dp[n - 1] = stones[n - 1];
        for(int i = n - 2; i >= 0; --i){
            dp[i] = dp[i + 1] + stones[i];
        }
        int pos = 2, ans = stones[0] + stones[1], sumB = 0;
        while(pos < n){
            int target = dp[pos], index = pos;
            for(int i = pos; i <= n; ++i){
                if(target >= dp[i]){
                    index = i;
                    target = dp[index];
                }
            }
            if(index == n) ans += dp[pos] - dp[index];
            pos = index;
            target = dp[pos], index = pos;
            for(int i = pos; i <= n; ++i){
                if(target <= dp[i]){
                    index = i;
                    target = dp[index];
                }
            }
            if(index == n) ans -= dp[pos] - dp[index];
            pos = index;
        }
        return ans;
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {12,4}, {8,1}, {6,3}};
    vector<int> b = { -1,2,-3,4,-5 };
     vector<int> b_ = { 9,3,5,1,7,4 };

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
    so.stoneGameVIII(b);
}
