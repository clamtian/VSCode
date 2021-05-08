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
    int res = INT_MAX;
    int minimumTimeRequired(vector<int>& jobs, int k) {
        sort(jobs.begin(), jobs.end());
        int n = jobs.size();
        vector<int> sum(k, 0);
        sum[0] = jobs[n - 1];
        fill(jobs, sum, n - 2, k - 1, jobs[n - 1]);
        return res;
    }
    void fill(vector<int>& jobs, vector<int>& sum, int pos, int space, int maxSum){
        if(pos == -1 || pos + 1 == space) {
            res = min(res, maxSum);
            return;
        }
        int r = sum.size() - space;
        for(int i = 0; i < r; ++i){
            if(sum[i] + jobs[pos] >= res) continue;
            sum[i] += jobs[pos];
            fill(jobs, sum, pos - 1, space, max(maxSum, sum[i]));
            sum[i] -= jobs[pos];
        }
        if(r < sum.size()){
            sum[r] = jobs[pos];
            fill(jobs, sum, pos - 1, space - 1, max(maxSum, sum[r]));
            sum[r] -= jobs[pos];
        }
        
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {3,4},{1,100},{2,2},{5,5} };
    vector<int> b = { 1,2,4,7,8 };
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
    so.minimumTimeRequired(b, 2);
}
