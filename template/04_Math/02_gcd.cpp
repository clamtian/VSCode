#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 欧几里得算法求最大公约数

int gcd(int a, int b){
    return b ? gcd(b, a % b) : a;
}

