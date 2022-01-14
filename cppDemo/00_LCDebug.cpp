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

typedef pair<int, int> PII;
class Solution {
public:
        bool possibleToStamp(vector<vector<int>>& grid, int h, int w) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> Sum(m + 1, vector<int>(n + 1, 0));
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                Sum[i + 1][j + 1] = grid[i][j] + Sum[i][j + 1] + Sum[i + 1][j] - Sum[i][j];
            }
        }
        vector<vector<PII>> g(m + 1, vector<PII>(n + 1));
        for(int i = m; i > 0; --i){
            for(int j = n; j > 0; --j){
                //cout << i << "  " << j << endl;
                if(i >= h && j >= w && Sum[i][j] + Sum[i - h][j - w] - Sum[i - h][j] - Sum[i][j - w] == 0){
                    g[i][j].first = h, g[i][j].second = w;
                }else if(grid[i - 1][j - 1] != 1){
                    //cout << i << " " << j << endl;
                    if(i < m && grid[i][j - 1] != 1) 
                        g[i][j].first = g[i + 1][j].first - 1, g[i][j].second = g[i + 1][j].second - 1;
                    if(j < n && grid[i - 1][j] != 1) 
                        g[i][j].first = max(g[i][j].first, g[i][j + 1].first - 1), g[i][j].second = max(g[i][j].second, g[i][j + 1].second - 1);
                    //cout << g[i][j].first << " " << g[i][j].second << endl;
                    if(g[i][j].first == -1 || g[i][j].second == -1) return false;
                }
            }
        }
        return true;
    }
};

const int N = 25;
int son[N][26], cnt[N], idx = 0;
    void addWord(string word) {
        int p = 0, n = word.size();
        for(int i = 0; i < n; ++i){
            int u = word[i] - 'a';
            if(!son[p][u]) son[p][u] = ++idx;
            p = son[p][u];
        }
        cnt[p++];
    }
    
    bool dfs(string& word, int p, int u){
        if(u == word.size()) return true;
        if(word[u] == '.'){
            for(int i = 0; i < 26; ++i){
                if(son[p][i] && dfs(word, son[p][i], u + 1))
                    return true;
            }
            return false;
        }
        if(!son[p][word[u] - 'a']) return false;
        return dfs(word, son[p][word[u] - 'a'], u + 1);
    }
    bool search(string word) {
        return dfs(word, 0, 0);
    }

int main() {

    
    cout << search("bad") << endl;
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

    TreeNode* r0 = new TreeNode(0);
    TreeNode* r1 = new TreeNode(1);
    TreeNode* r2 = new TreeNode(2);
    TreeNode* r3 = new TreeNode(3);
    TreeNode* r4 = new TreeNode(4);
    TreeNode* r5 = new TreeNode(5);

    r0->left = r1;
    r1->right = r2;
    r0->right = r3;
    //r3->left = r4;
    //r3->right = r5;


    string t = "alex";
    string s = "ababaab";
    char ch = 49 + '0';
    vector<bool> is(2, true);
    vector<string> str = { "E23", "2X2", "12S" };
    vector<vector<string>> matrix = { {"1", "0", "1", "0", "0"},
                                      {"1", "0", "1", "1", "1"},
                                      {"1", "1", "1", "1", "1"},
                                      {"1", "0", "0", "1", "0"} };


    vector<vector<int>> a = {{1,0,0,0},{1,0,0,0}, { 1,0,0,0}, {1,0,0,0}};
    vector<int> b = { 2,2,3,4 };
    vector<char> vc = { 'A', 'A', 'A', 'B', 'B', 'B' };
    vector<int> b_ = { 11,14,15,7,5,5,6,10,11,6 };

    vector<vector<char>> ch_vec = { {'1', '1', '1'},
                               {'0', '1', '0'},
                               {'1', '1', '1'} };
    vector<vector<string>> svec = { {"David","3","Ceviche"},
                                {"Corina","10","Beef Burrito"},
                                {"David","3","Fried Chicken"},
                                {"Carla","5","Water"},
                                {"Carla","5","Ceviche"},
                                {"Rous","3","Ceviche"} };
    Solution so;
    so.possibleToStamp(a, 4, 3);
    return 0;
}
