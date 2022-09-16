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


int lock_t = 0;

int testAndSet(int* lock, int new_val) {
    int old_val = *lock;
    *lock = new_val;
    return old_val;    
}


void lock(int* lock_t) {
    while (testAndSet(lock_t, 1) == 1) {
        /*
         * 保存当前进程的状态 S
         * 把 S 插入到阻塞队列（获取不到当前锁而阻塞）
         * 陷入内核态  执行进程调度
         */
    }
}

void unlock(int* lock_t) {
    *lock_t = 0;
    // 取出阻塞队列队头  执行
}













int main(int argc,char* argv[])
{
	int a[10] = { [0] = 1, [2] = 5};
	return 1;
}