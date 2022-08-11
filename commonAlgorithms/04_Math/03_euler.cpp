#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 求欧拉函数

// 1∼N 中与 N 互质的数的个数被称为欧拉函数
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


//筛法求欧拉函数

//给定一个正整数 n，求 1∼n 中每个数的欧拉函数之和。

/*
当p是素数时

1.   phi(p)=p-1。（不多bb）

2.   phi(p^k)=(p-1)*p^(k-1)

证明：

令n == p^k,小于 n 的正整数共有 p^k-1 个,其中与 p 不互素的个数共 p^(k-1)-1 个，它们是 1*p,2*p,3*p ... (p^(k-1)-1)*p

所以phi(p^k) == (p^k-1) - (p^(k-1)-1) == p^k - p^(k-1) == (p-1) * p^(k-1)。

3.   如果i%p==0,那么phi(i*p)=p*phi(i);(自行理解)

4.   如果i%p!=0,那么phi(i*p)=(p-1)*phi(i);

证明：

因为欧拉函数是积性函数，所以有phi(i*p)=phi(i)*phi(p),然后p是素数，所以phi(i*p)=(p-1)*phi(i);

*/

int primes[N], cnt;     // primes[]存储所有素数
int euler[N];           // 存储每个数的欧拉函数
bool st[N];         // st[x]存储x是否被筛掉

void get_eulers(int n){
    euler[1] = 1;
    for (int i = 2; i <= n; i ++ )
    {
        if (!st[i]){
            primes[cnt ++ ] = i;
            euler[i] = i - 1;
        }
        for (int j = 0; primes[j] <= n / i; j ++ ){
            int t = primes[j] * i;
            st[t] = true;
            if (i % primes[j] == 0){
                euler[t] = euler[i] * primes[j];
                break;
            }
            euler[t] = euler[i] * (primes[j] - 1);
        }
    }
}
