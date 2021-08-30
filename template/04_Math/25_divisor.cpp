#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 试除法求约数

vector<int> getDivisors(int x){
    vector<int> res;
    for(int i = 1; i <= x / i; ++i){
        if(x % i == 0) {
            res.push_back(i);
            if(x / i != i) res.push_back(x / i);
        }
    }
    pow(1, 2);
    sort(res.begin(), res.end());
    return res;
}


/*
如果 N = p1^c1 * p2^c2 * ... *pk^ck
约数个数： (c1 + 1) * (c2 + 1) * ... * (ck + 1)
约数之和： (p1^0 + p1^1 + ... + p1^c1) * ... * (pk^0 + pk^1 + ... + pk^ck)
*/

// 给定 n 个正整数 ai，请你输出这些数的乘积的约数个数，答案对 10^9+7 取模。

void divide(int x){
    if(x < 2) return;
    for(int i = 2; i <= x / i; ++i){
        while(x % i == 0){
            ++umap[i];
            x /= i;
        }
    }
    if(x > 1) ++umap[x];
}


// 给定 n 个正整数 ai，请你输出这些数的乘积的约数之和，答案对 10^9+7 取模。

/*
费马小定理

    a ^ (p - 1) = 1(% p)  p为质数
    即 a * a ^ (p - 2) = 1(% p)
    在模运算中没有除法，一个数在模意义下乘以某个数再%这个模意义下的数得到的答案是1，那么‘某个数’就是这个数的逆元。
    逆元可以代替除法，除以这个数就等于乘以这个数的逆元。
    所以a的逆元为 a ^ (p - 2)

*/
int main(){
    int n = 0;
    cin >> n;
    int x = 0;
    while(n--){
        cin >> x;
        divide(x);
    }
    long long ans = 1;
    for(auto p : umap){
        long long inv = (fastPow(p.first, p.second + 1) - 1) % MOD * fastPow(p.first - 1, MOD - 2) % MOD;
        ans = ans * inv % MOD;
    } 
    cout << ans << endl;
    return 0;
}
