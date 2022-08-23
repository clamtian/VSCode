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

typedef long int LL;

using namespace std;



const int N = 100001;

int q[N];
int size = 0;

int pop() {
    int res = q[0];
    swap(q[0], q[--size]);
    int idx = 0, l = 1;
    while (l < size) {
        int t = l + 1 < size && q[l + 1] > q[l] ? l + 1 : l;
        t = q[idx] > q[t] ? idx : t;
        if (t == idx) break;
        swap(q[t], q[idx]);
        idx = t;
        l = idx * 2 + 1;
    }
    return res;
}



void insert(int x) {
    int t = size;
    q[size++] = x;
    while (q[t] > q[(t - 1) / 2]) {
        swap(q[t], q[(t - 1) / 2]);
        t = (t - 1) / 2;
    }

}

int main(int argc,char* argv[])
{
	vector<int> v = { 3, 5, 5, 7,6,56,87};
	for (int i = 0; i < v.size(); ++i) {
		insert(v[i]);
	}
	for (int i = 0; i < v.size(); ++i) {
		cout << pop() << endl;
	}
	return 1;
}