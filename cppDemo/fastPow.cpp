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

using namespace std;

int MOD = 1e9 + 7;
//快速幂
class Solution {
public:
    int fastPow(int base, int pow){
        int res = 1;
        while(pow){
            if(pow & 1) res = res * base % MOD;
            base = base * base % MOD;
            pow >>= 1;
            
        }
        return res;
    }
};