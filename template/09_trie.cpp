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

//找出每个数左边离它最近的比它大/小的数
vector<int> monStack(vector<int>& nums){
    stack<int> sta;
    vector<int> res;
    int n = nums.size();
    for(int i = 0; i < n; ++i){
        while(!sta.empty() && sta.top() >= nums[i]){
            sta.pop();
        }
        if(sta.empty()) res.push_back(-1);
        else res.push_back(sta.top());
        sta.push(nums[i]);
    }
    return res;
}


void monQueue(vector<int>& nums, int k){
    int n = nums.size();
    deque<int> q1, q2;

    //输出窗口中最小值  维护递增队列
    for(int i = 0; i < n; ++i){
        if(!q1.empty() && q1.front() <= i - k) q1.pop_front();
        while(!q1.empty() && nums[q1.back()] > nums[i]) q1.pop_back();
        q1.push_back(i);
        if(i >= k - 1) printf("%d ", nums[q1.front()]);
        
    }


    printf("\n");

    //输出窗口中最大值  维护递减队列
    for(int i = 0; i < n; ++i){
        
        if(!q2.empty() && q2.front() <= i - k) q2.pop_front();
        while(!q2.empty() && nums[q2.back()] < nums[i]) q2.pop_back();
        q2.push_back(i);
        if(i >= k - 1) printf("%d ", nums[q2.front()]);
    }
}

int main(){
    vector<int> nums = { 1, 3, -1, -3, 5, 3, 6, 7 };
    monQueue(nums, 3);

    /*
    -1 -3 -3 -3 3 3 
    3  3  5  5  6 7 
    */
    return 1;
}