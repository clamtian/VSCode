#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 求欧拉函数


int phi(int x){
    if(x < 2) return x;
    int res = x;
    for(int i = 2; i <= x / i; ++i){
        if(x % i == 0){
            res = res / i * (i - 1);
            while(x % i == 0) x /= i;
        }
    }
    if(x > 1) res = res / x * (x - 1);
    return res;
}
