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


#define N 50

int main(){
    pid_t pid = 0;

    if((pid = fork()) == 0){
        cout << "pause" << endl;
        pause();
        cout << "control should never reach here!" << endl;
        exit(0);
    }

    sleep(2);
    kill(pid, SIGCONT);
    sleep(2);
    exit(0);
}
int main2(){
    int status = 0;
    pid_t pid = 0;

    cout << "Hello" << endl;

    pid = fork();

    cout << !pid << endl;

    if(pid){
        if(waitpid(-1, &status, 0) > 0){
            if(WIFEXITED(status)){
                cout << WEXITSTATUS(status) << endl;
            }
        }
    }
    cout << "Bye" << endl;

    exit(2);
}
int main1()
{
    int status, i;
    pid_t pid;

    for(int i = 0; i < N; ++i){
        if((pid = fork()) == 0){
            exit( 100 + i );
        }
    }

    while((pid = waitpid(-1, &status, 0)) > 0){
        if(WIFEXITED(status)){
            cout << "child " << pid << " terminated normally with exit status = " << WEXITSTATUS(status) << endl; 
        }else{
            cout << "child " << pid << " terminated abnormally" << endl;
        }
    }

    if(errno != ECHILD){
        cout << "waitpid error" << endl;
    }

    exit(0);
    
    return 1;
}