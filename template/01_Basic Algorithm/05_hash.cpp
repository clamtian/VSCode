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

// 前26个素数
vector<int> dict = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 
                     47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101 };

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