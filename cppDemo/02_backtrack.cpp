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

/*
 * ����һ�� û���ظ� ���ֵ����У����������п��ܵ�ȫ���С�
 * 
 * ʾ��:
 * 
 * ����: [1,2,3]
 * ���:
 * [
 *   [1,2,3],
 *   [1,3,2],
 *   [2,1,3],
 *   [2,3,1],
 *   [3,1,2],
 *   [3,2,1] ]
*/

//û���ظ����֣�ֻ���¼�±��Ƿ��ù�
class Solution1 {
public:
    vector<vector<int>> res;
    vector<vector<int>> permute(vector<int>& nums) {
        int n = nums.size();
        vector<bool> is_used(n, false);
        vector<int> vec;
        if (n == 0) return res;
        dfs(vec, nums, is_used);
        return res;
    }
    void dfs(vector<int> vec, vector<int>& nums, vector<bool>& is_used) {
        if (vec.size() == nums.size()) {
            res.push_back(vec);
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (!is_used[i]) {
                vector<int> cop = vec;
                cop.push_back(nums[i]);
                is_used[i] = true;
                dfs(cop, nums, is_used);
                is_used[i] = false;
            }
        }
    }
};

/*
 * ����һ���ɰ����ظ����ֵ����� nums ��������˳�򷵻����в��ظ���ȫ���С�
 * 
 * ʾ����
 * ���룺nums = [1,1,2]
 * �����
 * [[1,1,2],
 *  [1,2,1],
 *  [2,1,1]]
*/

//���ظ����֣���Ҫͬʱ��¼���ֺ��±�
class Solution2 {
public:
    vector<vector<int> > res;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        int n = nums.size();
        vector<bool> is_used(n, false);
        vector<int> vec;
        dfs(vec, nums, is_used);
        return res;
    }
    void dfs(vector<int> vec, vector<int>& nums, vector<bool>& is_used) {
        if (vec.size() == nums.size()) {
            res.push_back(vec);
            return;
        }
        unordered_set<int> hashset;
        for (int i = 0; i < nums.size(); ++i) {
            if (!is_used[i] && hashset.find(nums[i]) == hashset.end()) {
                vector<int> cop = vec;
                cop.push_back(nums[i]);
                is_used[i] = true;
                dfs(cop, nums, is_used);
                is_used[i] = false;
                hashset.insert(nums[i]);
            }
        }
    }
};

