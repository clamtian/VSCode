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

const int N = 100010;
int son[N][26], cnt[N], idx = 0;

// 0号点既是根节点，又是空节点
// son[][]存储树中每个节点的子节点
// cnt[]存储以每个节点结尾的单词数量


// 插入一个字符串
void insert(string& s){
    int p = 0, n = s.size();
    for(int i = 0; i < n; ++i){
        int u = s[i] - 'a';
        if(!son[p][u]) son[p][u] = ++idx;
        p = son[p][u];
    }
    cnt[p]++;
}


// 查询字符串出现的次数
int search(string& s){
    int p = 0, n = s.size();
    for(int i = 0; i < n; ++i){
        int u = s[i] - 'a';
        if(!son[p][u]) return 0;
        p = son[p][u];
    }
    return cnt[p];
}


int main(){
    
    int n = 0;
    cin >> n;
    string op, str;
    while(n--){
        cin >> op >> str;
        if(op == "I") insert(str);
        else{
            int c = search(str);
            cout << c << endl;
        }
    }
    return 0;
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