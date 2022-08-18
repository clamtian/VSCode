#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

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
	int a = 1;
	vector<int> v{ 9 };
	test(v);
	v = test(v);
	test(test(v));
	return 1;
}