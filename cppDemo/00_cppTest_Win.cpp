#include <string.h>
#include <vector>
#include <string>
#include <bitset>

#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 
#include<iomanip>
using namespace std;

int main(){
    int m;
    cin >> m;
    char op;
    vector<int> e, ne;
    int pos, val, head = -1;
    
    while(m--){
        cin >> op;
        switch(op){
            case 'D':
                cin >> pos;
                if(pos == 0) head = ne[head];
                else{
                    ne[pos - 1] = ne[ne[pos - 1]];
                } 
                break;
            case 'H':
                cin >> val;
                e.push_back(val);
                ne.push_back(head);
                head = e.size() - 1;
                break;
            case 'I':
                cin >> pos >> val;
                e.push_back(val);
                ne.push_back(ne[pos - 1]);
                ne[pos - 1] = e.size() - 1;
                break;
        }
    }
    
    while(head != -1){
        cout << e[head] << " ";
        head = ne[head];
    }
    
    
    return 0;
}