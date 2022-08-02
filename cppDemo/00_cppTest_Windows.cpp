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

class C : public A, public B {
	//void funcA();
};


void A::funcA () {
	cout << "A's funcA" << endl;
}
void B::funcA () {
	cout << "B's funcA" << endl;
}


 
int main(int argc,char* argv[])
{
	C c;
	c.funcA();
	return 1;
}