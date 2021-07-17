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


//离散化
int find(vector<pair<int, int>>& A, int x){
    int l = 0, r = A.size();
    //寻找第一个小于x的值的下标
    while(l < r){
        int mid = l + r >> 1;
        if(A[mid].first <= x) l = mid + 1;
        else r = mid;
    }
    return l - 1;  //返回最后一个 <= x 的值的下标
    //return l;  //返回第一个 > x 的值的下标
}


int main(){
    
    
    return 1;
}