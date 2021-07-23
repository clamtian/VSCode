#include <iostream>
#include <vector>

using namespace std;

const int N = 10;

int q[N], ph[N];
int s = 0, tot = 1;

int insert(int x){
    int idx = s;
    q[s++] = x;
    while(q[idx] < q[(idx - 1) / 2]){
        swap(q[idx], q[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
    return idx;
}

void pop(int idx){
    int l = idx * 2 + 1;
    while(l < s){
        int t = l + 1 < s && q[l + 1] < q[l] ? l + 1 : l;
        t = q[t] < q[idx] ? t : idx;
        if(t == idx) break;
        swap(q[t], q[idx]);
        idx = t;
        l = idx * 2 + 1;
    }
}

int main(){
    int n = 0;
    cin >> n;
    string op;
    int a = 0, b = 0;
    while(n--){
        cin >> op;
        if(op == "I"){
            cin >> a;
            ph[tot++] = insert(a);
        }else if(op == "PM"){
            cout << q[0] << endl;
        }else if(op == "DM"){
            swap(q[0], q[--s]);
            pop(0);
        }else if(op == "D"){
            cin >> a;
            swap(q[ph[a]], q[--s]);
            pop(ph[a]);
        }else{
            cin >> a >> b;
            q[ph[a]] = b;
            pop(ph[a]);
        }
    }
    return 0;
}