#include <vector>
#include <string>
#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

string addStrings(string num1, string num2) {
    
    if(num1.size() < num2.size()) return addStrings(num2, num1);
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());
    string res = "";
    int t = 0, m = num1.size(), n = num2.size();
    for(int i = 0; i < m; ++i){
        t += num1[i] - '0';
        if(i < n) t += num2[i] - '0';
        res.push_back(t % 10 + '0');
        t /= 10;
    }
    if(t) res.push_back(t + '0');
    reverse(res.begin(), res.end());
    return res;
}


int main(){
    
    return 1;
}