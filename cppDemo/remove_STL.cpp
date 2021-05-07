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

int main(){
    vector<int> vec = { 3,6,9,1,1,5,8,1 };
    string str = "36911581";
    list<int> li(vec.begin(), vec.end());
    map<int, int> m = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6} };
    set<int> s = { 1, 2, 3, 4, 5, 6 };


    //remove函数
    auto vec1 = vec;
    auto str1 = str;
    auto li1 = li;
    remove(vec1.begin(), vec1.end(), 1);//{ 3,6,9,5,8,5,8,1 };
    remove(str1.begin(), str1.end(), '1');//"36958581"
    li1.remove(1);//{ 3,6,9,5,8 };

    //删除某元素
    auto vec2 = vec;
    auto str2 = str;
    auto li2 = li;

    vec2.erase(remove(vec2.begin(), vec2.end(), 1), vec2.end());//{ 3,6,9,5,8 }
    str2.erase(remove(str2.begin(), str2.end(), '1'), str2.end());//"36958"
    li2.remove(1);//{ 3,6,9,5,8 }

    //删除大于5的元素
    auto vec3 = vec;
    vec3.erase(remove_if(vec3.begin(), vec3.end(), [](int a) {return a > 3;}), vec3.end());//{ 3, 1, 1, 1 }

    //关联容器删除value大于3的元素
    auto m1 = m;
    auto s1 = s;

    for(auto it = m1.begin(); it != m1.end();){
        if(it->second > 3){
            m1.erase(it++);
        }else{
            ++it;
        }
    }
    //{ {1, 1}, {2, 2}, {3, 3} };
    for(auto it = s1.begin(); it != s1.end();){
        if(*it > 3){
            s1.erase(it++);
        }else{
            ++it;
        }
    }
    //{ 1, 2, 3}
    return 1;
}