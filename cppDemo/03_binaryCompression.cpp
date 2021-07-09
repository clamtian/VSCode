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
* ������״̬ѹ��
* ���ظ�����A��B��Ԫ��c --> int A,B   c = 0 ~ 31
* A�в���c
  A |= (1<<c)
* A��ȥ��c
  A &= ~(1<<c)
  A ^= (1<<c)
* A B �ϲ�
  A | B
* �ж�B�ǲ���A���Ӽ�
  return (A&B) == B
* �ж�c�ڲ���A��
  return A & (1<<c)
* lowbit(lowbit()��������ȡһ�����������λ��һ���ߵ�0��ɵ���) ��:12��1100����lowbit(12) = 4(100)
  return x & (-x);
* ö��A��ȫ���Ӽ�
  for(int i = A; i; i = (i-1) & A)
  {
    //do something
  }
* �ж�һ�����ǲ���2���ݴ�
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