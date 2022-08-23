TCP和UDP

1. 首部：TCP不包含可选项20字节，UDP固定8字节
2. 连接方式：面向连接；不需要连接
3. 可靠性：应答机制，保证可靠；不保证数据的交付
4. 传送机制：面向字节流，没有保护边界；基于数据包，有保护边界
5. 连接控制：流量控制、拥塞控制；没有
6. 服务形式：一对一；一对一、一对多、多对多
7. 分片机制：大于MSS分片；不分片，大于MTU时在IP层进行分片




```javascript

const int N = 100001;

int q[N];
int size = 0;

int pop() {
    int res = q[0];
    swap(q[0], q[size--]);
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
    while (t > 0 && q[t] > q[(t - 1) / 2]) {
        swap(q[t], q[(t - 1) / 2]);
        t = (t - 1) / 2;
    }

}









```








