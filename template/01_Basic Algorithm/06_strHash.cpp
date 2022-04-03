#include <iostream>
#include <string>
#include <vector>

typedef unsigned long long ULL;

using namespace std;

/*
核心思想：将字符串看成P进制数，P的经验值是131或13331，取这两个值的冲突概率低
小技巧：取模的数用2^64，这样直接用unsigned long long存储，溢出的结果就是取模的结果
*/

int P = 13331;

// 返回字符串的hash值
ULL getHashVal(string& s){
    ULL ans = 0;
    for(int i = 0; i < s.size(); ++i)
        ans += (ans * P + s[i]);
    return ans;
}

// 返回字符串hash值的数组形式
vector<ULL> getHashVec(string& s){
    int n = s.size();
    // h[k]存储字符串前k个字母的哈希值, p[k]存储 P^k mod 2^64
    vector<ULL> h(n + 1), p(n + 1);
    p[0] = 1;
    for (int i = 1; i <= n; ++i){
        h[i] = h[i - 1] * P + s[i - 1];
        p[i] = p[i - 1] * P;
    }
    // h[i]代表子字符串[0, i - 1]的hash值
    return h;
}

// 计算子串 str[l ~ r] 的哈希值 
ULL get(int l, int r, vector<ULL>& h, vector<ULL>& p){
    return h[r + 1] - h[l] * p[r - l + 1];
}

int main(){
    
    return 0;
}