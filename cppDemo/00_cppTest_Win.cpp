
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

typedef long int LL;

using namespace std;

inline int com(int a, int b) { 
    for (int i = 0; i < a; ++i) 
        cout << i << endl;
    return a > b;
}
int main() {
    int a = 9, b = 8;
    com(a, b);
    //cout << a << endl;
    return 0;
}