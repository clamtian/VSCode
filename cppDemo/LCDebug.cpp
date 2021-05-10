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
    int maxSumMinProduct(vector<int>& nums) {
        int MOD = 1e9 + 7;
        int n = nums.size();
        vector<long long> sum(n + 1, 0);
        for(int i = 0; i < n; ++i){
            sum[i + 1] = sum[i] + nums[i];
        } 
        vector<int> left(n, -1), right(n, -1);
        stack<int> s;
        for(int i = 0; i < n; ++i){
            while(!s.empty() && nums[i] < nums[s.top()]){
                right[s.top()] = i;
                s.pop();
            }
            s.push(i);
        }
        while(!s.empty()){
            right[s.top()] = n;
            s.pop();
        }
        for(int i = n - 1; i >= 0; --i){
            while(!s.empty() && nums[i] < nums[s.top()]){
                left[s.top()] = i;
                s.pop();
            }
            s.push(i);
        }
        while(!s.empty()){
            left[s.top()] = 0;
            s.pop();
        }
        long long ans = 0;
        for(int i = 0; i < n; ++i){
            ans = max(ans, nums[i] * (sum[right[i]] - sum[left[i]] - nums[left[i]]));
        }
        return ans % MOD;
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {3,4},{1,100},{2,2},{5,5} };
    vector<int> b = { 2,5,4,2,4,5,3,1,2,4 };
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
    vector<string> str = { "leet", "code" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    string st = ", 234 ;";
    cout << atoi(st.c_str()) << endl;
    //cout << so.func(4, a) << endl;
    so.maxSumMinProduct(b);
}
