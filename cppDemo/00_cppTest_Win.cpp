#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <climits>
#include <bitset>
#include <algorithm>

using namespace std;

struct myStruct{
    uint16_t a;
    myStruct *next;
};



int main(){
    myStruct s[3];
    s[0].a = 1, s[0].next = &s[1], s[1].a = 9, s[1].next = NULL;
    myStruct *p1, *p2;
    p1 = &s[0], p2 = &s[2];
    cout << sizeof(s[0]) << endl;
    cout << sizeof(p1) << endl;
    cout << p1 << endl;
    cout << p2 << endl;
    cout << p2 - p1 << endl;
    cout << ((p2 - p1) << 12) << endl;

}

