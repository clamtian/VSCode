#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

typedef long int LL;

using namespace std;

class A {
public:
	virtual void funcA();
	void funcB();

	
};
class B : public A {
public:
	//virtual void funcA();
	void funcB();


};

class C : public B {
	virtual void funcA();
};

void C::funcA () {
	cout << "C's funcA" << endl;
}

void A::funcA () {
	cout << "A's funcA" << endl;
}
void A::funcB () {
	cout << "A's funcB" << endl;
}


void B::funcB () {
	cout << "B's funcB" << endl;
}
 
int main(int argc,char* argv[])
{
	A *a = new C();
	a->funcA();
	return 1;
}