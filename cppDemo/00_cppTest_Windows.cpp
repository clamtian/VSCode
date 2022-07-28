#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

typedef long int LL;

using namespace std;

	class Age   
	{   
	public:   
	  
	    Age& operator++() //前置++   
	    {   
	        ++i;   
	        return *this;   
	    }   
	  
	    const Age operator++(int) //后置++   
    {   
	        Age tmp = *this;   
	        ++(*this);  //利用前置++   
            cout << &(tmp) << endl;
	        return tmp;   
	    }   
	  
	    Age& operator=(int i) //赋值操作   
	    {   
	        this->i = i;   
	        return *this;   
	    }   
	  
	private:   
	    int i;   
	};  
int main() {
    const int s = 1;
    int s2 = s;
	Age a;   
    Age c = a++;
    const Age& b = a++;
    cout << &a << endl;
    cout << &b << endl;
    return 0;
}