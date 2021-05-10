#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
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
    int num = 0;

    cout << num << endl;

    int pid = fork();

    if(pid == 0){
        cout << num + 1 << endl;
    }else{
        cout << num - 1 << endl;
    }
    
    return 1;
}