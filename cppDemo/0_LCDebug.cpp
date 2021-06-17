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
    int minOperationsToFlip(string s) {
        stack<pair<int, int>> s1;
        stack<char> s2;
        int n = s.size(), pos = 0;
        while(pos < n){
            if(s[pos] == '(' || s[pos] == '&' || s[pos] == '|'){
                s2.push(s[pos]);
            }else{
                if(s[pos] == '1'){
                    pair<int, int> p = { 1, 0 };
                    s1.push(p);
                }else if(s[pos] == '0'){
                    pair<int, int> p = { 0, 1 };
                    s1.push(p);
                }
                if(s2.size() > 0 && s2.top() != '('){
                    char op = s2.top();
                    s2.pop();
                    auto p1 = s1.top();
                    s1.pop();
                    auto p2 = s1.top();
                    s1.pop();
                    auto p = cal(op, p1, p2);
                    s1.push(p);
                }
            }
            ++pos;
        }
        while(s2.size() > 0){
            char op = s2.top();
            s2.pop();
            auto p1 = s1.top();
            s1.pop();
            auto p2 = s1.top();
            s1.pop();
            auto p = cal(op, p1, p2);
            s1.push(p);
        }
        auto p = s1.top();
        return p.first == 0 ? p.second : p.first;
    }
    pair<int, int> cal(char op, pair<int, int>& p1, pair<int, int>& p2){
        pair<int, int> p = { INT_MAX, INT_MAX };
        if(op == '|'){
            p.first = min(p.first, p1.first + p2.first);
            p.first = min(p.first, p1.second + p2.first + 1);
            p.first = min(p.first, p1.first + p2.second + 1);
            p.second = min(p.second, p1.second + p2.second);
            p.second = min(p.second, p1.second + p2.first);
            p.second = min(p.second, p1.first + p2.second);
        }else if(op == '&'){
            p.first = min(p.first, p1.first + p2.second);
            p.first = min(p.first, p1.second + p2.first);
            p.first = min(p.first, p1.first + p2.first);
            p.second = min(p.second, p1.second + p2.second);
            p.second = min(p.second, p1.second + p2.first + 1);
            p.second = min(p.second, p1.first + p2.second + 1);
        }
        return p;
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
    so.minOperationsToFlip("1|(0&(1))");

    return 0;
}
