// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include <algorithm>
#include "commoncppproblem472.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=472 lang=cpp
 *
 * [472] 连接词
 */

// @lc code=start
class Solution {
public:
    class Trie {
    private:
        vector<Trie*> children;
        bool isEnd;
    public:
        Trie() : children(26), isEnd(false) {}
    
        void insert(const string& word) {
            Trie* node = this;
            for(char c : word){
                int index = c - 'a';
                if(!node->children[index]){
                    node->children[index] = new Trie();
                }
                node = node->children[index];
            }
            node->isEnd = true;
        }
        bool search(const string& word) {
            Trie* node = this;
            for(char c : word){
                int index = c - 'a';
                if(!node->children[index]){
                    return false;
                }
                node = node->children[index];
            }
            return node->isEnd;
        }
    };
    vector<string> findAllConcatenatedWordsInADict(vector<string>& words) {
        sort(words.begin(), words.end(), [](const string& s1, const string& s2){ return s1.size() < s2.size(); });
        Trie trie;
        trie.insert(words[0]);
        int n = words.size(), minLen = words[0].size();
        int i = 1;
        for(; i < n; ++i){
            if(words[i].size() == minLen){
                trie.insert(words[i]);
            }else{
                break;
            }
        }
        vector<string> res;
        while(i < n){
            vector<int> dp(words[i].size() + 1, 0);
            dp[0] = 1;
            for(int j = 1; j <= words[i].size(); ++j){
                for(int k = j - 1; k >= 0; --k){
                    if(dp[k] && trie.search(words[i].substr(k, j - k))){
                        dp[j] = 1;
                        break;
                    }
                }
            }
            if(dp.back()) res.push_back(words[i]);
            trie.insert(words[i++]);
        }
        return res;
    }
};
// @lc code=end

