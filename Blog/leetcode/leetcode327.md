# leetcode 327

```
给你一个整数数组 nums 以及两个整数 lower 和 upper 。求数组中，值位于范围 [lower, upper] （包含 lower 和 upper）之内的 区间和的个数 。

区间和 S(i, j) 表示在 nums 中，位置从 i 到 j 的元素之和，包含 i 和 j (i ≤ j)。
```

解法1:使用归并排序的思想

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
        //若l==r，则区间的长度是0，直接返回
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

解法2：线段树
```JavaScript


```