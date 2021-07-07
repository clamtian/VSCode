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

bool check(int k){/* ... */}
/*
* mid打到左端点 往右推 寻找最后一个的时候使用
*/
int binarySearch(int l, int r){
    while(l < r){
        int mid = (l + r) >> 1;
        if(check(mid)) r = mid;
        else l = mid + 1;
    }
    return l;
}

/*
* mid打到右端点 往左推 寻找第一个的时候使用
*/
int binarySearch(int l, int r){
    while(l < r){
        int mid = (l + r + 1) >> 1;
        if(check(mid)) l = mid;
        else r = mid - 1;
    }
    return l;
}


int main(){
    
    return 1;
}