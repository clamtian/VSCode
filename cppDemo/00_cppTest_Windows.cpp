#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <time.h>

typedef long int LL;

using namespace std;

class A {
public:
	void funcA();

	
};
class B : public A {
public:
	//virtual void funcA();
	void funcA();


};


void A::funcA () {
	cout << "A's funcA" << endl;
}
void B::funcA () {
	cout << "B's funcA" << endl;
}


float sqrt(float);
A sqrt(double);

const vector<int> test(const vector<int>& v) {
	vector<int> s = v;
	//v.push_back(1);
	return s;
}
int main(int argc,char* argv[])
{
	clock_t start,end;//定义clock_t变量
	start = clock();//开始时间

	string s = "";
	for (int i = 0; i  < 100000; ++i) s += to_string(i / 10 + i);

	string c = s;
	end = clock();   //结束时间
	cout<<"time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;  //输出时间（单位：ｓ）
	return 1;
}