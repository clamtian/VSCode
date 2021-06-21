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
    vector<string> readBinaryWatch(int turnedOn) {
        vector<string> res;
        for(int i = 0; i <= turnedOn; ++i){
            vector<string> hour = getHour(i);
            vector<string> minute;
            getMinute(turnedOn - i, 0, 5, minute);
            for(auto& h : hour){
                for(auto& m : hour){
                    string time = h + ":" + m;
                    res.push_back(time);
                }
            }
        }
        return res;
    }
    void getMinute(int n, int m, int b, vector<string>& minute){
        if(n == 0){
            if(m < 10){
                minute.push_back("0" + to_string(m));
            }else if(m < 60){
                minute.push_back(to_string(m));
            }
            return;
        }
        if(b == -1) return;
        getMinute(n - 1, m ^ (1 << b), b - 1, minute);
        getMinute(n, m, b - 1, minute);
    }
    vector<string> getHour(int n){
        vector<string> res;
        if(n == 0){
            res.push_back("0");
        }else if(n == 1){
            res.push_back("1");
            res.push_back("2");
            res.push_back("4");
            res.push_back("8");
        }else if(n == 3){
            res.push_back("7");
            res.push_back("11");
        }
        return res;
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
    string s = "ababaab";
    char ch = 49 + '0';
    vector<bool> is(2, true);
    vector<string> str = { "ab", "ba", "ba" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    so.readBinaryWatch(2);

    return 0;
}
