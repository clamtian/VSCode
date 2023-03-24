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
#include <unistd.h>  
#include <stdio.h>  
#include <ctime>

typedef long int LL;

using namespace std;


int main ()   
{   
    int tot = 10000;
    int f = 1;
    srand((int)time(0));  // 产生随机种子  把0换成NULL也行
    while (tot >= 5000 && tot < 100000) {
        int k = 100;
        tot -= 100;
        int r = 1;
        while (r && tot >= 5000 && tot < 100000) {
            
            r = rand() % 2;
            if (r) {
                tot += 100;
            }
            cout << "epoll " << f++ << ": tot = " << tot << endl;
        }
        while (r == 0 && tot > 0 && tot >= 5000 && tot < 100000) {
            k *= 2;
            r = rand() % 2;
            if (r == 0) tot -= k;
            else tot += k;
            cout << "epoll " << f++ << ": tot = " << tot << endl;
        }
        
    }
    return 0;
} 















 //int a[10] = { [0] = 1, [2] = 5};