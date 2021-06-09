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
//#include <priority_queue>
#include <numeric>
#include <queue>
#include <sstream> 

using namespace std;

//compare(const T a, const T b) 若b的优先级比a大返回true，否则返回false


//自定义比较函数 定义int值小的优先级大
bool compare(const int a, const int b){
    return a > b;
}

//比较类
struct cmp{
    bool operator()(const int a, const int b){
        return a > b;
    }
};

int main(){
    vector<int> vec = { 9, 3, 5, 1, 7, 4 };

    //1.自定义比较函数来定义sort排序规则
    //sort(vec.begin(), vec.end(), compare); //{ 9, 7, 5, 4, 3, 1 }

    //2.使用自定义比较类
    //sort(vec.begin(), vec.end(), cmp());

    //3.使用lambda表达式
    //sort(vec.begin(), vec.end(), [](const int a, const int b){return a > b;});
    
    //1.使用lambda表达式自定义优先级队列
    auto cm = [](const int a, const int b){ return a > b; };//cm是一个函数指针，指向lambda表达式

    cout << typeid(cm).name() << endl;
    priority_queue<int, vector<int>, decltype(cm)> q(cm, vec);
    
    //2.使用函数指针自定义优先级队列
    priority_queue<int, vector<int>, decltype(compare) *> q1(compare, vec);
    
    return 1;
}