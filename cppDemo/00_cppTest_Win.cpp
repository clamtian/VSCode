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

void quickSort(vector<int>& nums, int l, int r){
    if(l >= r) return;
    int i = l - 1, j = r + 1, x = nums[l + r >> 1];
    while(i < j){
        do ++i; while(nums[i] < x);
        do --j; while(nums[j] > x);
        if(i < j) swap(nums[i], nums[j]);
    }
    quickSort(nums, l, j);
    quickSort(nums, j + 1, r);
}


int main(){
    int n;
    scanf("%d", &n);
    vector<int> nums(n, 0);
    for(int i = 0; i < n; ++i) scanf("%d", &nums[i]);
    quickSort(nums, 0, n - 1);
    for(int i = 0; i < n; ++i) printf("%d ", &nums[i]);
    return 0;
}