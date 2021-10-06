#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>
#include <bitset>
#include <algorithm>

using namespace std;

int main(){
    int x = -1, a = 15;
    bitset<32> c(((a << 28) & x) >> 28);
    cout << c << endl;

}

