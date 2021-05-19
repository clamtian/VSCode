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
    int kthLargestValue(vector<vector<int>>& matrix, int k) {
        vector<int> vec;
        int m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> xor_vec(m + 1, vector<int>(n + 1, 0));
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                xor_vec[i + 1][j + 1] = xor_vec[i][j + 1] ^ xor_vec[i + 1][j] ^ matrix[i][j];
            }
        }
        for(int i = 1; i <= m; ++i){
            for(int j = 1; j <= n; ++j){
                vec.push_back(xor_vec[i][j]);
            }
        }
        sort(vec.begin(), vec.end());
        int pos = m * n;
        while(k > 0){
            --pos;
            --k;
        }
        return vec[pos];
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {5,2},{1,6}};
    vector<int> b = { 2, 3, 1, 6, 7 };
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
    so.kthLargestValue(a, 4);
}
