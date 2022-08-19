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

vector<int> generateRandomArray(int maxSize, int maxValue){
    srand((unsigned)time(NULL));
    int n = rand() % (maxSize + 2) - 1; 
    vector<int> res(n, 0);
    for(int i = 0; i < n; ++i){
        res[i] = rand() % (maxValue + 2) - 1;
        res[i] *= rand() % 2 == 0 ? 1 : -1; 
    }
    return res;
}

int main(){
    vector<int> vec = generateRandomArray(20, 100);
    auto vec2 = vec;
    
}