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

class Solution {
public:
    int coins1(vector<int>& coins, int aim){
        int n = coins.size();
        if(aim <= 0) return aim == 0 ? 1 : 0;
        vector<vector<int>> map(n + 1, vector<int>(aim + 1, -1));
        return process(coins, 0, aim);
    }
    int process(vector<int>& coins, int pos, int aim, vector<vcctor<int>>& map){
        if(map[pos][aim] != -1) return map[pos][aim];
        int n = coins.size(), ans = 0;
        if(pos == n){
            ans = aim == 0 ? 1 : 0;
        }else{
            for(int i = 0; i * coins[pos] <= aim; ++i){
                int value = map[pos + 1][aim - i * coins[pos]];
                if(value != -1){
                    ans += value;
                }else{
                    ans += process(coins, pos + 1, aim - i * coins[pos], map);
                }
            }
        }
        map[pos][aim] = ans;
        return ans;
    }
};