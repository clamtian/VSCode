#include <iostream>
#include <string>

typedef unsigned long long ULL;

using namespace std;

/*
核心思想：将字符串看成P进制数，P的经验值是131或13331，取这两个值的冲突概率低
小技巧：取模的数用2^64，这样直接用unsigned long long存储，溢出的结果就是取模的结果
*/

const int N = 100010;
int P = 13331;

ULL h[N], p[N]; // h[k]存储字符串前k个字母的哈希值, p[k]存储 P^k mod 2^64

// 计算子串 str[l ~ r] 的哈希值
ULL get(int l, int r){
    return h[r] - h[l - 1] * p[r - l + 1];
}

int main(){
    int n = 0, m = 0;
    cin >> n >> m;
    string s;
    cin >> s;
    
    //初始化
    p[0] = 1;
    for (int i = 1; i <= n; ++i){
        h[i] = h[i - 1] * P + s[i - 1];
        p[i] = p[i - 1] * P;
    }

    int l1 = 0, r1 = 0, l2 = 0, r2 = 0;
    while(m--){
        cin >> l1 >> r1 >> l2 >> r2;
        ULL h1 = get(l1, r1);
        ULL h2 = get(l2, r2);
        if(h1 == h2) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
    return 0;
}