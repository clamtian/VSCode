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
    vector<int> maximizeXor(vector<int>& nums, vector<vector<int>>& queries) {
        int n = queries.size();
        vector<int> res(n, -1), tmp(n, 0);
        unordered_map<int, set<int>> m;
        for(int k = 30; k >= 0; --k){
            for(int num : nums){
                m[num].insert(num >> k);
            }
            for(int i = 0; i < n; ++i){
                if((queries[i][0] >> k) > 0 || (queries[i][1] >> k) > 0){
                    int target = (tmp[i] * 2 + 1) ^ (queries[i][0] >> k);
                    int l = queries[i][1] >> k;
                    if(m.find(target) != m.end() && *(m[target].begin()) <= queries[i][1]){
                        tmp[i] = tmp[i] * 2 + 1;
                    }else{
                        tmp[i] = tmp[i] * 2;
                    }
                }
            }
            m.clear();
        }
        unordered_set<int> s1(nums.begin(), nums.end());
        for(int i = 0; i < n; ++i){
            int val = queries[i][0] ^ tmp[i];
            if(s1.find(val) != s1.end()){
                res[i] = tmp[i];
            }
        }
        return res;
    }
};

int main() {

    string str1 = "ac";
    string str2 = "aba";
    cout << max(str1, str2) << endl;
    vector<vector<int>> a = { {536870912,262144}};
    vector<int> b = { };
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
    so.maximizeXor(b, a);
}
