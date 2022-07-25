#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

typedef long int LL;

using namespace std;

int sum(int& a, int b) { 
    int p = a + b;
    cout << &a << endl;
    return a; 
}
int main() {
    cout << "hello" << endl;
    int s = 0, d = 9;

    cout << sum(s, d) << endl;
    cout << sum(s, d) << endl;
    return 0;
}