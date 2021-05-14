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


vector<vector<int>> deleteRow(vector<vector<int>>& vecs, set<int>& rows){
    vector<vector<int>> res;
    int n = vecs.size();
    for(int i = 0; i < n; ++i){
        if(rows.find(i) == rows.end()){
            res.push_back(vecs[i]);
        }
    }
    return res;
}
int main()
{
    vector<vector<int>> a = { { 3, 4, 5, 2 },
                              { 1, 1, 4, 5 },
                              { 2, 2, 1, 2 },
                              { 5, 5, 5, 5 },
                              { 5, 5, 5, 5 },
                              { 5, 5, 5, 5 } };
    string res('a', 3);
    return 1;
}