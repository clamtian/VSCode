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


// C = A + B A >= 0 B >= 0
vector<int> add(vector<int> &A, vector<int> &B){
    if (A.size() < B.size()) return add(B, A);
    vector<int> C;
    int t = 0;
    for (int i = 0; i < A.size(); i ++ ){
        t += A[i];
        if (i < B.size()) t += B[i];
        C.push_back(t % 10);
        t /= 10;
    }

    if (t) C.push_back(t);
    return C;
}

// C = A - B  A >= B A >= 0 B >= 0
vector<int> sub(vector<int> &A, vector<int> &B){
    vector<int> C;
    for (int i = 0, t = 0; i < A.size(); i ++ ){
        t = A[i] - t;
        if (i < B.size()) t -= B[i];
        C.push_back((t + 10) % 10);
        if (t < 0) t = 1;
        else t = 0;
    }

    while (C.size() > 1 && C.back() == 0) C.pop_back();
    return C;
}

// C = A * b A >= 0 b >= 0
vector<int> mul(vector<int> A, int b){
    
    vector<int> C;
    int t = 0;
    for (int i = 0; i < A.size() || t; ++i){
        if (i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t /= 10;
    }
    while (C.size() > 1 && C.back() == 0) C.pop_back();

    return C;
}


vector<int> generateArndomArray(int maxSize, int maxValue){
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
    
    vector<int> num1 = generateArndomArray(12, 9);
    vector<int> num2 = generateArndomArray(13, 9);

    for(int& n : num1) n = abs(n);
    for(int& n : num2) n = abs(n);

    vector<int> res = sub(num1, num2);
    
    return 1;
}