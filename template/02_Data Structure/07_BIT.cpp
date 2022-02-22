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
#define lowbit(x) ((x) & (-x))
using namespace std;

// 树状数组：快速修改以及查询前缀和


// 更新节点值
void update(int i, int x, vector<int>& tree){
    for (int p = i; p < tree.size(); p += lowbit(p))
        tree[p] += x;
}

// 查询前缀和
int query(int i, vector<int>& tree){
    int s = 0;
    for (int p = i; p; p -= lowbit(p))
        s += tree[p];
    return s;
}

int main(){
    int n = 10010;
    vector<int> tree(n);
    vector<int> nums = { 2,0,1,3 };
    for(int i = 0; i < nums.size(); ++i){
        update(nums[i], 1, tree);
        cout << query(nums[i], tree) << endl;
    }
    return 1;
}