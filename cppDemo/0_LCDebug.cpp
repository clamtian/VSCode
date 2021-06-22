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
class Solution {
public:
    vector<int> pathsWithMaxScore(vector<string>& board) {
        vector<int> res = { 0, 0 };
        int n = board.size();
        vector<vector<LL>> val(n, vector<LL>(n, 0));
        vector<vector<LL>> path(n, vector<LL>(n, 0));
        for(int i = n - 2; i >= 0; --i){
            if(board[i][n - 1] != 'X'){
                val[i][n - 1] = val[i + 1][n - 1] + (board[i][n - 1] - '1');
                path[i][n - 1] = 1;
            }else{
                for(; i >= 0; --i){
                    val[i][n - 1] = -1;
                    path[i][n - 1] = 0;
                }
            }     
        }
        for(int j = n - 2; j >= 0; --j){
            if(board[n - 1][j] != 'X'){
                val[n - 1][j] = val[n - 1][j + 1] + (board[n - 1][j] - '1');
                path[n - 1][j] = 1;
            }else{
                for(; j >= 0; --j){
                    val[n - 1][j] = -1;
                    path[n - 1][j] = 0;
                }
            }     
        }
        for(int i = n - 2; i >= 0; --i){
            for(int j = n - 2; j >= 0; --j){
                if(board[i][j] != 'X'){
                    if(path[i + 1][j] > 0){
                        val[i][j] = val[i + 1][j];
                        path[i][j] = path[i + 1][j];
                    }
                    if(path[i][j + 1] > 0){
                        if(val[i][j + 1] > val[i][j]){
                            val[i][j] = val[i][j + 1];
                            path[i][j] = path[i][j + 1];
                        }else if(val[i][j + 1] == val[i][j]){
                            path[i][j] += path[i][j + 1];
                        }
                    }
                    if(path[i + 1][j + 1] > 0){
                        if(val[i + 1][j + 1] > val[i][j]){
                            val[i][j] = val[i  +1][j + 1];
                            path[i][j] = path[i + 1][j + 1];
                        }else if(val[i + 1][j + 1] == val[i][j]){
                            path[i][j] += path[i + 1][j + 1];
                        }
                    }
                    if(board[i][j] != 'E') val[i][j] += (board[i][j] - '1');
                }else if(board[i][j] != 'E'){
                    val[i][j] = -1;
                    path[i][j] = 0;
                }
            }
        }
        if(path[0][0] == 0) return res;
        res[0] = val[0][0] % MOD;
        res[1] = path[0][0] % MOD;
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
    vector<string> str = { "E23", "2X2", "12S" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };

    Solution so;
    so.pathsWithMaxScore(str);

    return 0;
}
