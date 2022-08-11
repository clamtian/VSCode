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

/*
* 重点是下标与位置(第几个)的关系
*/
void prefix(vector<vector<int>>& A){
    int m = A.size(), n = A[0].size();
    vector<vector<int>> B(m + 1, vector<int>(n + 1, 0));  //B[i][j] 表示前i行j列的和
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            B[i + 1][j + 1] = B[i + 1][j] + B[i][j + 1] + A[i][j] - B[i][j];
        }
    }
}

//B = { 0, b1, b2, b3, ... }
void diff(vector<int>& B){
    int n = B.size();
    vector<int> A(n, 0);  //B是A的前缀和 
    for(int i = 0; i < n; ++i){
        A[i] = B[i + 1] - B[i];
    }
}
// 二维数组差分
void diff(vector<vector<int>>& B){
    int m = B.size(), n = B[0].size();
    vector<vector<int>> A(m, vector<int>(n, 0));
    for(int i = 0; i < m; ++i){
        for(int j = 0; j < n; ++j){
            A[i][j] = B[i + 1][j + 1] - B[i][j + 1] - B[i + 1][j] + B[i][j];
        }
    }
}

int main(){

    return 1;
}