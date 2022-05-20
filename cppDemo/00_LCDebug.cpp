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
typedef unsigned long long ULL ;
class Solution {
public:
    vector<string> maxNumOfSubstrings(string s) {
        int n = s.size();
        unordered_map<char, int> umap;
        for (int i = 0; i < n; ++i) {
            umap[s[i]] = i;
        }
        return maxNumOfSubstrings(s, umap, 0, n);
    }
    vector<string> maxNumOfSubstrings(string& s, unordered_map<char, int>& umap, int p, int q) {
        int i = p, j = p + 1;
        while (i < j) {
            j = max(j, umap[s[i]]);
            ++i;
        }
        if (j + 1 >= q) return maxNumOfSubstrings(s, umap, p + 1, q);
        if (j <= p + 1) return vector<string>{ s.substr(p, j + 1 - p) };
        vector<string> v1 = maxNumOfSubstrings(s, umap, p + 1, j + 1);
        vector<string> v2 = maxNumOfSubstrings(s, umap, j + 1, q);
        if (!v1.size()) v1.push_back(s.substr(p, j + 1 - p));
        v1.insert(v1.end(), v2.begin(), v2.end());
        return v1;
    }
};

int main() {

    
    ///cout << search("bad") << endl;
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
    vector<int> b = { 8,9,8,6,1,1 };
    vector<int> b_ = { 5,2,4,3,1,0 };
    vector<char> vc = { 'A', 'A', 'A', 'B', 'B', 'B' };
    

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
    so.maxNumOfSubstrings("adefaddaccc");
    return 0;
}
