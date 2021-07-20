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

int main(){
    
    int *p;
    allocator<int> al;
    p = al.allocate(1);
    cout << *p << endl;
    cout << p << endl;
    //al.deallocate(p, 1);


    cout << *p << endl;
    cout << p << endl;
    
    delete p;
    cout << *p << endl;
    cout << p << endl;
    return 0;
}