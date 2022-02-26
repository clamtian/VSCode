# leetcode 327

给你一个整数数组 nums 以及两个整数 lower 和 upper 。求数组中，值位于范围 [lower, upper] （包含 lower 和 upper）之内的 区间和的个数 。

区间和 S(i, j) 表示在 nums 中，位置从 i 到 j 的元素之和，包含 i 和 j (i ≤ j)。

## 提示：
    1 <= nums.length <= 10^5
    -2^31 <= nums[i] <= 2^31 - 1
    -10^5 <= lower <= upper <= 10^5
    题目数据保证答案是一个 32 位 的整数


## 解法1:使用归并排序的思想

根据题意很容易想到前缀和，而最显而易见的解法便是逐个下标对比，时间复杂度是O(n^2)，对于一道困难题来说这个复杂度肯定是不能接受的，必须将其优化到O(nlogn)及以下。

给定下标i和j，假设区间[i, j]是满足题目要求的区间，固定j，可以看出，i是否满足条件跟i的位置无关，只与i处的数值有关。这就启示我们可以使用归并排序的思想来解决问题，在归并的过程中将左侧携带的信息传给右侧。

```Javascript
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long long> sum(n + 1, 0);
        for(int i = 0; i < n; ++i)
            sum[i + 1] = sum[i] + nums[i];
        return merge(sum, lower, upper, 0, n);
    }
    int merge(vector<long long>& sum, int lo, int u, int l, int r){
        // 若l==r，则区间的长度是0，直接返回
		if(l >= r) return 0;
        int mid = (l + r) >> 1;
        int cnt = merge(sum, lo, u, l, mid) + merge(sum, lo, u, mid + 1, r);
		//固定左端点，找右端点的范围
        int i = mid + 1, j = mid + 1;
        for(int k = l; k <= mid; ++k){
            while(i <= r && sum[i] < sum[k] + lo) ++i;
            while(j <= r && sum[j] <= sum[k] + u) ++j;
            cnt += j - i;
        }
		/*
		//也可以改成固定右端点，找左端点的范围
		int i = l, j = l;
        for(int k = mid + 1; k <= r; ++k){
            while(i <= mid && sum[i] < sum[k] - u) ++i;
            while(j <= mid && sum[j] <= sum[k] - lo) ++j;
            cnt += j - i;
        }
		*/
        i = l, j = mid + 1;
        int k = 0;
        vector<long long> m(r - l + 1, 0);
        while(i <= mid && j <= r){
            if(sum[i] < sum[j]) m[k++] = sum[i++];
            else m[k++] = sum[j++];
        }
        while(i <= mid) m[k++] = sum[i++];
        while(j <= r) m[k++] = sum[j++];
        for(int i = l; i <= r; ++i)
            sum[i] = m[i - l];
        return cnt;
    }
};
```

## 解法2：树状数组

遍历前缀和数组sum并记录出现过的数字，当遍历到数字s时，我们需要查找在[s - upper, s - lower]之中出现过的数字个数，我们可以使用树状数组来对数据进行记录并且修改。

最直观的记录方法是开一个数组对出现过的数字做下标记录，但数字范围过大，所以需要对所有可能出现过的数字进行哈希处理。

```JavaScript
typedef long long LL;
class Solution {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        int ans = 0;
        set<LL> s;
        s.insert(0);
        vector<LL> sum(n + 1);
        for(int i = 0; i < n; ++i){
            sum[i + 1] = sum[i] + nums[i];
            s.insert(sum[i + 1]);
            s.insert(sum[i + 1] - upper);
            s.insert(sum[i + 1] - lower);
        }
        unordered_map<LL, int> umap;
        int idx = 0;
        for(LL num : s)
            umap[num] = idx++;
        vector<int> tree(idx + 1);
        for(int i = 0; i <= n; ++i){
            int a = query(umap[sum[i] - upper], tree), b = query(umap[sum[i] - lower] + 1, tree);
            ans += b - a;
            for(int p = umap[sum[i]] + 1; p <= idx; p += p & -p) tree[p] += 1;
        }
        return ans;
    }
    int query(int i, vector<int>& tree){
        int s = 0;
        for(int p = i; p; p -= p & -p) s += tree[p];
        return s;
    }
};
```