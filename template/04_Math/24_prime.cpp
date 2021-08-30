#include <iostream>
#include <vector>

using namespace std;

// 试除法判定质数

bool is_prime(int x){
    if (x < 2) return false;
    for (int i = 2; i <= x / i; i ++ )
        if (x % i == 0)
            return false;
    return true;
}



// 试除法求质因数
// 给定 n 个正整数 ai，将每个数分解质因数，并按照质因数从小到大的顺序输出每个质因数的底数和指数。
void divide(int x){
    for(int i = 2; i <= x / i; ++i){
        if(x % i == 0){
            int s = 0;
            while(x % i == 0) x /= i, ++s;
            cout << i << " " << s << endl;
        }
    }
    if(x > 1) cout << x << " " << 1 << endl;  //x是质数时判定生效
}


// 朴素筛法求素数  O(nlogn)

const int N = 100010;
int primes[N], cnt;     // primes[]存储所有素数
bool st[N];         // st[x]存储x是否被筛掉

void get_primes(int n){
    for (int i = 2; i <= n; i ++ ){
        if (st[i]) continue;
        primes[cnt ++ ] = i;
        for (int j = i + i; j <= n; j += i)
            st[j] = true;
    }
}


// 线性筛法求素数 O(n)

int primes[N], cnt;     // primes[]存储所有素数
bool st[N];         // st[x]存储x是否被筛掉

void get_primes(int n){
    for (int i = 2; i <= n; i ++ ){
        if (!st[i]) primes[cnt ++ ] = i;
        for (int j = 0; primes[j] <= n / i; j ++ ){
            st[primes[j] * i] = true;
            if (i % primes[j] == 0) break;
        }
    }
}






int main(){
    return 0;
}