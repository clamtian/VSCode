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


char* getM() {
	//char p[] = "Hello";
	//return p;
}
int main(int argc,char* argv[])
{
	char p[] = "nullptr;";
	//p = getM();
	cout << p << endl;
	return 1;
}