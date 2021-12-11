
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

typedef long int LL;

using namespace std;

int main(){
    int n;
    cin >> n;
    vector<vector<LL>> vec(2 * n);
    int pos = 0;
    for(int i = 0; i < n; ++i){
        LL a, b;
        scanf("%ld%ld", &a, &b);
        vec[pos++] = { a, 0 };
        vec[pos++] = { b, 1 };
    }
    sort(vec.begin(), vec.end());
    vector<LL> res(n + 1, 0);
    LL cnt = 0;
    vector<LL> v = { 0 ,0 };
    for(int i = 0; i < n * 2; ++i){
        auto c = vec[i];
        if(c[1] == 0) res[cnt] += c[0] - v[0] - 1, res[cnt + 1]++, ++cnt;
        else res[cnt] += c[0] - v[0], --cnt;
        v = c;
    }
    for(int i = 1; i <= n; ++i){
        printf("%ld ", res[i]);
    }
    return 0;
}