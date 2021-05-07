#include <iostream>
#include <vector> 
#include <algorithm>
using namespace std;

class IsGreaterThanThresholdFunctor
{
public:
	explicit IsGreaterThanThresholdFunctor(int tmp_threshold) : threshold(tmp_threshold) {}
    bool operator() (int num) const
    {
            return num > threshold ? true : false;
    }
private:
    const int threshold;
};

int RecallFunc(int *start, int *end, IsGreaterThanThresholdFunctor myFunctor)
{
    int count = 0;
    for(int *i = start; i != end + 1; i++)
    {
        count = myFunctor(*i) ? count + 1 : count;
    }
    return count;
}


int main()
{

    int a[5] = { 10,100,11,5,19 };
    int result = RecallFunc(a, a + 4, IsGreaterThanThresholdFunctor(55));
    cout<<result<<endl;

}