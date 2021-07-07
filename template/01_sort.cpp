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


void mergeSort(vector<int>& nums, int l, int r){
    if(l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(nums, l, mid);
    mergeSort(nums, mid + 1, r);
    int i = l, j = mid + 1, k = 0;
    vector<int> vec(r - l + 1, 0);
    while(i <= mid && j <= r){
        if(nums[i]< nums[j]) vec[k++] = nums[i++];
        else vec[k++] = nums[j++];
    }
    while(i <= mid) vec[k++] = nums[i++];
    while(j <= r) vec[k++] = nums[j++];

    for(i = 0; i < r - l + 1; ++i) 
        nums[i + l] = vec[i];
}


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
    
    return 1;
}