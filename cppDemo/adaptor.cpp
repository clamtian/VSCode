#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

//自定义less仿函数
//只有继承unary_function和binary_function才可以使用适配器
class MyLess : public binary_function<int, int, bool>{
public:
    bool operator()(const int a, const int b)const {
        return a < b;
    }
};

class MyLess30 : public unary_function<int, bool>{
public:
    bool operator()(const int a)const {
        return a < num;
    }
private:
    const int num = 30;
};

int main()
{
    int numbers[] = { 10,20,30,40,50,10 };
    int cx;
    cx = count_if(numbers, numbers + 6, bind2nd(MyLess(), 40));
    cout << "There are " << cx << " elements that are less than 40.\n";
    cx = count_if(numbers, numbers + 6, not1(MyLess30()));
    cout << "There are " << cx << " elements that are not less than 30.\n";
    return 0;
}