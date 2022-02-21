# leetcode 327

给你两个下标从 0 开始且长度为 n 的整数数组 nums1 和 nums2 ，两者都是 [0, 1, ..., n - 1] 的 排列 。

好三元组 指的是 3 个 互不相同 的值，且它们在数组 nums1 和 nums2 中出现顺序保持一致。换句话说，如果我们将 pos1v 记为值 v 在 nums1 中出现的位置，pos2v 为值 v 在 nums2 中的位置，那么一个好三元组定义为 0 <= x, y, z <= n - 1 ，且 pos1x < pos1y < pos1z 和 pos2x < pos2y < pos2z 都成立的 (x, y, z) 。

请你返回好三元组的总数目。

### 示例 1：
    输入：nums1 = [2,0,1,3], nums2 = [0,1,2,3]
    输出：1
    解释：
    总共有 4 个三元组 (x,y,z) 满足 pos1x < pos1y < pos1z ，分别是 (2,0,1) ，(2,0,3) ，(2,1,3) 和 (0,1,3) 。
    这些三元组中，只有 (0,1,3) 满足 pos2x < pos2y < pos2z 。所以只有 1 个好三元组。

### 解法1:使用归并排序的思想。

```Javascript

```

### 解法2：线段树

#### 提示1：
如果nums1的形式为[0,1,2,...,n-1]的话，问题会简单许多。是不是可以想办法将nums1转换成上面的形式？

#### 解答：
定义一个转换函数P(x) = pos1x，因为要求的三元组数目只与数字的位置有关，而与具体数字无关，所以将nums2做相应的转换并不影响结果(仔细思考)
nums1 = [2,0,1,3], nums2 = [0,1,2,3]
将两者做相应的转换：[0,1,2,3] [1,2,0,3]并不影响好三元组的数量
而经过转换后的数组对，要求(x,y,z)，只需要在nums2中固定y的位置，寻找y之前小于y数字的数量以及y后大于y的数字的数量即可，我们可以使用树状数组进行快速修改以及查询前缀和。

```JavaScript
#define lowbit(x) ((x) & (-x))
class Solution {
public:
    long long goodTriplets(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<int> p(n);
        for(int i = 0; i < n; ++i) p[nums1[i]] = i;
        long long ans = 0;
        vector<long long> tree(n + 1, 0);
        for(int i = 0; i < n; ++i){
            long long x = query(p[nums2[i]] + 1, tree);
            ans += x * (n - p[nums2[i]] - 1 - i + x);
            update(p[nums2[i]] + 1, 1, tree);
        }
        return ans;
    }
    void update(int i, int x, vector<long long>& tree){
        for (int pos = i; pos < tree.size(); pos += lowbit(pos))
            tree[pos] += x;
    }
    int query(int n, vector<long long>& tree){
        int ans = 0;
        for (int pos = n; pos; pos -= lowbit(pos))
            ans += tree[pos];
        return ans;
    }
};
```