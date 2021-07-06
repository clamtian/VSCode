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
    vector<vector<string>> displayTable(vector<vector<string>>& orders) {
        
        map<string, int> m;
        for(auto& order : orders){
            m[order[2]] = 0;
        }
        int n = m.size();
        vector<map<string, int>> vec(505, m);
        vector<int> flag(505, 0);
        for(auto& order : orders){
            flag[stoi(order[1])] = 1;
            vec[stoi(order[1])][order[2]]++;
        }
        vector<vector<string>> res;
        vector<string> v = { "Table" };
        for(auto [food, num] : m) v.push_back(food);
        res.push_back(v);
        for(int i = 1; i < 505; ++i){
            if(flag[i]){
                vector<string> v;
                for(auto [food, num] : vec[i]) 
                    v.push_back(to_string(num));
                res.push_back(v);
            }
        }
        return res;
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
    so.displayTable(svec);
    return 0;
}
