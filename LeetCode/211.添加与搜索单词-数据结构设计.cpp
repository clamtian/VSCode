// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem211.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=211 lang=cpp
 *
 * [211] 添加与搜索单词 - 数据结构设计
 */

// @lc code=start
const int N = 2500010;
int son[N][26], cnt[N], idx = 0;
class WordDictionary {
public:
    
    WordDictionary() {

    }
    
    void addWord(string word) {
        int p = 0, n = word.size();
        for(int i = 0; i < n; ++i){
            int u = word[i] - 'a';
            if(!son[p][u]) son[p][u] == ++idx;
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
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */
// @lc code=end

