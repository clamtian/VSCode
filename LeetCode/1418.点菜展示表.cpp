/*
 * @lc app=leetcode.cn id=1418 lang=cpp
 *
 * [1418] 点菜展示表
 */

// @lc code=start

typedef unsigned long long ULL;
const int N = 25;
ULL h[N], p[N]; // h[k]存储字符串前k个字母的哈希值, p[k]存储 P^k mod 2^64
int P = 1331;


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
                vector<string> v = { to_string(i) };
                for(auto [food, num] : vec[i]) 
                    v.push_back(to_string(num));
                res.push_back(v);
            }
            
        }
        return res;
    }
};
// @lc code=end

