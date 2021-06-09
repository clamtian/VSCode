#include <string.h>
#include <vector>
#include <string>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>
using namespace std;


//LeetCode 990
class Solution {
public:
	class unionFindSet {
	public:
		unordered_map<char, char> fatherMap;
		unordered_map<char, int> sizeMap;

		unionFindSet(vector<char> nodes) {
			fatherMap.clear();
			sizeMap.clear();
			for (char node : nodes) {
				fatherMap[node] = node;
				sizeMap[node] = 1;
			}
		}
		void unions(char a, char b) {
			char aHead = findHead(a);
			char bHead = findHead(b);
			if (!(aHead == bHead)) {
				int aSetSize = sizeMap[aHead];
				int bSetSize = sizeMap[bHead];
				if (aSetSize <= bSetSize) {
					fatherMap[aHead] = bHead;
					sizeMap[bHead] = aSetSize + bSetSize;
				}
				else {
					fatherMap[bHead] = aHead;
					sizeMap[aHead] = aSetSize + bSetSize;
				}
			}
		}
		bool isSameSet(char a, char b) {
			return findHead(a) == findHead(b);
		}

	private:
		char findHead(char node) {
			char father = fatherMap[node];
			if (father != node) {
				father = findHead(father);
			}
			fatherMap[node] = father;
			return father;
		}
	};
	bool equationsPossible(vector<string>& equations) {
		vector<char> nodes;
		for (int i = 0; i < 26; ++i) {
			nodes.emplace_back(i + 'a');
		}
		unionFindSet u(nodes);
		for (int i = 0; i < equations.size(); ++i) {
			if (equations[i][1] == '=') {
				u.unions(equations[i][0], equations[i][3]);
			}
		}
		for (int i = 0; i < equations.size(); ++i) {
			if (equations[i][1] == '!' && u.isSameSet(equations[i][0], equations[i][3])) {
				return false;
			}
		}
		return true;
	}
};