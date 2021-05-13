#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
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
#include <errno.h>
#include <sys/types.h>    
#include <sys/wait.h>
using namespace std;


#define N 2
int main()
{
    int i = 0;
    pid_t pid;

    cout << "the beginning of the program" << endl;

    pid = fork();

    cout << " after a new process" << endl;
    if(pid == 0) {
        cout << "I am child process" << endl;
        exit(0);
    }

    cout << "I am father process" << endl;
    
    return 1;
}