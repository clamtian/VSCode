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

struct Node{
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};


int main(){
    vector<Node*> vec;
    int m;
    cin >> m;
    char op;
    Node* head, *node, *next, *newNode;
    int pos, val;
    while(m--){
        cin >> op;
        switch(op){
            case 'H' : 
                cin >> val;
                node = new Node(val);
                vec.push_back(node);
                node->next = head;
                head = node;
                break;
            case 'I':
                cin >> pos >> val;
                node = vec[pos - 1];
                newNode = new Node(val);
                newNode->next = node->next;
                node->next = newNode;
                vec.push_back(newNode);
                break;
            case 'D':
                cin >> pos;
                if(pos == 0) head = head->next;
                else{
                    node = vec[pos - 1];
                    next = node->next;
                    node->next = next->next;
                    break;
                }
        }
    }
    node = head;
    while(node){
        cout << node->val << " ";
        node = node->next;
    }
    return 0;
}