#include <string.h>
#include <vector>
#include <string>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;



//LeetCode 1178


/*
* 二进制状态压缩
* 非重复集合A、B，元素c --> int A,B   c = 0 ~ 31
* A中插入c
  A |= (1<<c)
* A中去除c
  A &= ~(1<<c)
  A ^= (1<<c)
* A B 合并
  A | B
* 判断B是不是A的子集
  return (A&B) == B
* 判断c在不在A里
  return A & (1<<c)
* lowbit(lowbit()函数用来取一个二进制最低位的一与后边的0组成的数) 例:12（1100），lowbit(12) = 4(100)
  return x & (-x);
* 枚举A的全部子集
  for(int i = A; i; i = (i-1) & A)
  {
    //do something
  }
* 判断一个数是不是2的幂次
  x & (x - 1) == 0
*/
class Solution {
public:
    vector<int> findNumOfValidWords(vector<string>& words, vector<string>& puzzles) {
        int n = puzzles.size(), m = words.size();
        vector<int> res(n, 0);
        unordered_map<int, int> hash;
        for (int i = 0; i < m; ++i) {
            int t = 0;
            for (char ch : words[i]) t |= (1 << ch - 'a');
            ++hash[t];
        }
        for (int i = 0; i < n; ++i) {
            int k = 0, t = puzzles[i][0] - 'a';
            for (char ch : puzzles[i]) k |= (1 << ch - 'a');
            for (int j = k; j; j = (j - 1) & k) {
                if (j & (1 << t)) res[i] += hash[j];
            }
        }
        return res;
    }
};