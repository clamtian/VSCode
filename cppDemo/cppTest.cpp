#include <string.h>
#include <vector>
#include <string>

#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 
using namespace std;

class Base
{
public:
    int pub_ele = 1;
    virtual void func(){
        cout << "Base func()! " << endl;
    };
protected: 
    int prot_ele = 2;
private:
    int priv_ele = 3;
};

class Derive : public Base
{
public:
    int ele = 0;
    void func(){
        cout << "Derive func()! " << endl;
    }
};


int main()
{
    int k = 0;
    int num = 2 / k;
    cout << 4 << endl;
    cout << num << endl;
    
    return 1;
}