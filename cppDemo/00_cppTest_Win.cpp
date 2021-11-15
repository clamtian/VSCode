#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

typedef long long LL;

using namespace std;

int MOD = 1e9+7;

int main(){
    vector<int> v = { 3, 6, 12, 15, 16, 19 };
    auto a = lower_bound(v.begin(), v.end(), 17);
    return 0;
    
}