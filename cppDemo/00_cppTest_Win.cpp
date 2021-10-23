#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

typedef long long LL;

using namespace std;

int MOD = 1e9+7;

int main(){
    int n, m; 
    cin >> n >> m;
    vector<vector<LL>> dp1(n + 1, vector<LL>(m, 0)), dp2(n + 1, vector<LL>(m, 0));
    for(int i = 1; i <= n; ++i) dp1[i][0] = n - i + 1 + dp1[i - 1][0];
    dp2[n][0] = n;
    for(int i = n - 1; i > 0; --i) dp2[i][0] = i + dp2[i + 1][0];
    for(int j = 1; j < m; ++j){
        for(int i = 1; i <= n; ++i){
            dp1[i][j] = dp1[i - 1][j] + dp1[i][j - 1] * (n - i + 1) % MOD;
        }
    }
    for(int j = 1; j < m; ++j){
        dp2[n][j] = dp2[n][j - 1];
        for(int i = n - 1; i > 0; --i){
            dp2[i][j] = dp2[i + 1][j] + dp2[i][j - 1] * i % MOD;
        }
    }
    LL ans = 0;
    for(int i = 1; i <= n; ++i){
        ans += dp1[i][m - 1] * dp2[i][m - 1];
        ans %= MOD;
    }
    cout << ans << endl;
    return 0;
    
}