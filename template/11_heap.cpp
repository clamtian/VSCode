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

//大根堆
const int N = 100010;

int q[N];
int s = 0;

void insert(int x){
    int idx = s;
    q[s++] = x;
    //while(q[idx] < q[(idx - 1) / 2]){   小根堆
    while(q[idx] > q[(idx - 1) / 2]){
        swap(q[idx], q[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

int pop(){
    int res = q[0];
    swap(q[0], q[--s]);
    int l = 1, idx = 0;
    while(l < s){
        //int t = l + 1 < s && q[l + 1] < q[l] ? l + 1 : l;
        //t = q[t] < q[idx] ? t : idx;          小根堆
        int t = l + 1 < s && q[l + 1] > q[l] ? l + 1 : l;
        t = q[t] > q[idx] ? t : idx;
        if(t == idx) break;
        swap(q[t], q[idx]);
        idx = t;
        l = idx * 2 + 1;
    }
    return res;
}







int main(){
    return 1;
}