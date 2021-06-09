#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

/*
bind1st()和bind2nd()都是把二元函数转化为一元函数，方法是绑定其中一个参数。
*/
int main()
{
    int numbers[] = { 10,20,30,40,50,10 };
    int cx;
    //bind2nd()是绑定第二个参数。
    cx = count_if(numbers, numbers + 6, bind2nd(less<int>(), 40));
    cout << "There are " << cx << " elements that are less than 40.\n";

    /*
    std::placeholders::_1 是占位符，标定这个是要传入的参数。
    所以bind()不仅可以用于二元函数，还可以用于多元函数，可以绑定多元函数中的多个参数，不想绑定的参数使用占位符表示。
    */
    cx = count_if(numbers, numbers + 6, bind(less<int>(), std::placeholders::_1, 40));
    cout << "There are " << cx << " elements that are less than 40.\n";

    //bind1st()是绑定第一个参数。
    cx = count_if(numbers, numbers + 6, bind1st(less<int>(), 40));
    cout << "There are " << cx << " elements that are not less than 40.\n";
    cx = count_if(numbers, numbers + 6, bind(less<int>(), 40, std::placeholders::_1));
    cout << "There are " << cx << " elements that are not less than 40.\n";


    system("pause");
    return 0;
}