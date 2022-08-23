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


void quickSort(vector<int>& vec, int l, int r){
    if(l >= r) return;
    int i = l - 1, j = r + 1, x = vec[l + r >> 1];
    while(i < j){
        do ++i; while(vec[i] < x);
        do --j; while(vec[j] > x);
        if(i < j) swap(vec[i], vec[j]);
    }

    // 为什么用j？ 考虑数组[3,5]，i最终的位置是1，这样输入永远是[l, i] = [0, 1]
    quickSort(vec, l, j);
    quickSort(vec, j + 1, r);
}


int main(){
    vector<int> nums = { 49, 59 ,88, 37, 98, 97, 68 ,54 ,31 ,3 };
    int n = nums.size();
    quickSort(nums, 0, n - 1);
    return 1;
}