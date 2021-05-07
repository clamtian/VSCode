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


class Trie {
private:
    vector<Trie*> children;
    bool isEnd;
public:
    /** Initialize your data structure here. */
    Trie() : children(26), isEnd(false) {

    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
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
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
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
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        Trie* node = this;
        for(char c : prefix){
            int index = c - 'a';
            if(!node->children[index]){
                return false;
            }
            node = node->children[index];
        }
        return true;
    }
};
