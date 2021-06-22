#include <string.h>
#include <vector>
#include <string>
#include <bitset>

#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 

using namespace std;

int main()
{
    string data_file = "./inputTest.txt";
    ifstream infile(data_file.c_str());
	if (!infile) {
		cout << "open nas file failure !" << endl;
		exit(0);
	}

	string line;

	//get TrianglesNum
	string str;
	getline(infile, line);
	stringstream sin(line);
    int num = 0;
    while(sin){
        sin >> str;
        if(str == "ss_prop1"){
            sin >> num;
            cout << num << endl;
            sin >> num; 
            sin >> num; 
        }
    }
    return 1;
}