#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int MOD = 1e9 + 7;

typedef long long LL;
//快速幂
LL fastPow(LL m, LL k, LL p){
    LL res = 1;
    while(k){
        if(k & 1) res = res * m % p;
        m = m * m % p;
        k >>= 1;
    }
    return res;
}

int main(){
    cout << fastPow(2, 44, MOD) << endl;
    return 0;
}
