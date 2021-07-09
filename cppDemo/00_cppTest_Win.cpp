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


const double eps = 1e-7;
int main()
{
    double x;
    cin >> x;
    double l = -x, r = x;
    while(r - l > eps){
        double mid = (l + r) / 2;
        if(mid * mid * mid > x) r = mid;
        else l = mid;
    }
    cout << fixed << setprecision(6) << l << endl;
    
    return 1;
}