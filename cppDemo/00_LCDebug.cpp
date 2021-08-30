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
const int N = 40;
const int M = 6;
int h[N], e[M], ne[M], st[N], dist[N], w[M], idx;

int ans[N];
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<int> dist(n, INT_MAX / 2), back(n, INT_MAX / 2), st(n, 0);
        dist[src] = 0;
        st[src] = 1;
        vector<vector<PII>> g(n);
        for(auto f : flights) g[f[0]].push_back({ f[1], f[2] });
        queue<int> q;
        q.push(0);
        while(!q.empty() && k-- >= 0){
            queue<int> q1;
            back = dist;
            while(!q.empty()){
                int a = q.front();
                q.pop();
                st[a] = 0;
                for(auto [b, w] : g[a]){
                    if(dist[b] > back[a] + w){
                        dist[b] = back[a] + w;
                        if(!st[b]){
                            q1.push(b);
                            st[b] = 1;
                        }
                    }
                }
            }
            q = q1;
        }
        return dist[dst] == INT_MAX / 2 ? -1 : dist[dst];
    }
};

int main() {

    
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


    vector<vector<int>> a = {{0,1,1},{0,2,5},{1,2,1},{2,3,1}};
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
    so.findCheapestPrice(4, a, 0 , 3, 1);
    return 0;
}
