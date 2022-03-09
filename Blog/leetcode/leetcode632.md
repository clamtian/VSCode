# leetcode 632

你有 k 个 非递减排列 的整数列表。找到一个 最小 区间，使得 k 个列表中的每个列表至少有一个数包含在其中。

我们定义如果 b-a < d-c 或者在 b-a == d-c 时 a < c，则区间 [a,b] 比 [c,d] 小。

## 示例 1：
    输入：nums = [[4,10,15,24,26], [0,9,12,20], [5,18,22,30]]
    输出：[20,24]
    解释： 
    列表 1：[4, 10, 15, 24, 26]，24 在区间 [20,24] 中。
    列表 2：[0, 9, 12, 20]，20 在区间 [20,24] 中。
    列表 3：[5, 18, 22, 30]，22 在区间 [20,24] 中。

## 解法1:贪心

区间包含每个列表中的至少一个数意味着最小区间一定是以列表中的某两个数作为开头和结尾的。

我们需要对整个列表中的数字进行遍历来寻找最小区间。具体可以在每个列表下挂一个指针，利用堆维护当前最小值，每个循环内将最小值弹出，然后将相同列表的下一个数填入堆内，同时更新堆中数据的最大值，利用这个最大值和最小值来更新最小区间。等某个列表遍历完成之后，由于堆中最小值也比这个列表中的最大值大，所以循环结束。
```Javascript
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int n = nums.size();
        vector<int> point(n);
        auto cm = [&](const int a, const int b){
            return nums[a][point[a]] > nums[b][point[b]];
        };

        priority_queue<int, vector<int>, decltype(cm)> pq(cm);
        int l = INT_MAX, r = INT_MIN;
        for(int i = 0; i < n; ++i){
            pq.push(i);
            l = min(l, nums[i][0]);
            r = max(r, nums[i][0]);
        }
        int q = r;
        while(pq.size()){
            int i = pq.top();
            pq.pop();
            ++point[i];
            if(point[i] == nums[i].size()) break;
            q = max(q, nums[i][point[i]]);
            pq.push(i);
            int p = nums[pq.top()][point[pq.top()]];
            if(r - l > q - p) r = q, l = p;
            else if(r - l == q - p && l > p)  r = q, l = p;
        }
        return { l, r };
    }
};
```

## 解法2

将问题进行**等价转化**：将所有列表保留下标进行排序，问题转化为求整个数组的最小长度，在该长度范围内每个列表中的数字至少出现一次。这样就将问题reduce到LeetCode76题。

```JavaScript
typedef pair<int, int> PII;
class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        int n = nums.size();
        vector<vector<int>> data;
        for(int i = 0; i < n; ++i)
            for(auto num : nums[i])
                data.push_back({ num, i });
        sort(data.begin(), data.end());
        vector<int> dict(n);
        vector<int> res = { data[0][0], data.back()[0] };
        int tot = 0;
        for(int i = 0, j = 0; j < data.size(); ++j){
            if(!dict[data[j][1]]) ++tot;
            ++dict[data[j][1]];
            while(tot == n && i <= j){
                vector<int> v = { data[i][0], data[j][0] };
                //cout << v[0] << " " << v[1] << endl;
                res = compare(res, v);
                if(dict[data[i][1]] == 1) --tot;
                --dict[data[i][1]];
                ++i;
            }
        }
        return res;
    }
    vector<int> compare(vector<int>& v1, vector<int>& v2){
        int a = v1[0], b = v1[1], c = v2[0], d = v2[1];
        if(b - a < d - c) return v1;
        else if(b - a > d - c) return v2;
        else if(a < c) return v1;
        return v2;
    }
};
```