#include <string.h>
#include <vector>
#include <string>
#include <bitset>

#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 
#include<iomanip>
using namespace std;

vector<int> sub(vector<int>& A, vector<int>& B){
    int m = A.size(), n = B.size();
    int t = 0;
    vector<int> res;
    for(int i = 0; i < m; ++i){
        t = A[i] - t;
        if(i < n) t -= B[i];
        res.push_back((t + 10) % 10);
        if(t < 0) t = 1;
        else t = 0;
    }
    while(res.size() > 1 && res.back() == 0) res.pop_back();
    
    return res;
}
bool cmp(vector<int>& A, vector<int>& B){
    int m = A.size(), n = B.size();
    if(m > n) return true;
    if(m == n){
        for(int i = m - 1; i >= 0; --i){
            if(A[i] < B[i]) return false;
            if(A[i] > B[i]) return true;
        }
    }
    return true;
}



int main(){
    
    string s1, s2;
    cin >> s1 >> s2;
    vector<int> num1, num2;
    for(int i = s1.size() - 1; i >= 0; --i) num1.push_back(s1[i] - '0');
    for(int i = s2.size() - 1; i >= 0; --i) num2.push_back(s2[i] - '0');
    vector<int> res;
    if(cmp(num1, num2)) res = sub(num1, num2);
    else{
        cout << "-";
        res = sub(num2, num1);
    }
    for(int i = res.size() - 1; i >= 0; --i) cout << res[i];
    
    return 0;
}