# 前置++与后置++

```Javascript
// 前缀形式：增加然后取回值
UPInt& UPInt::operator++()
{
    *this += 1; // 增加
    return *this; // 取回值
}
// postfix form: fetch and increment
const UPInt UPInt::operator++(int)
{
    UPInt oldValue = *this; // 取回值
    ++(*this); // 增加
    return oldValue; // 返回被取回的值
}
```
重载函数间的区别决定于它们的参数类型上的差异，但是不论是increment或decrement的前缀还是后缀都只有一个参数。为了解决这个语言问题，C++规定后缀形式有一个int类型参数，当函数被调用时，编译器传递一个0做为int参数的值给该函数。

前缀形式返回一个引用，后缀形式返回一个const类型

## 1、为什么后置返回对象，⽽不是引⽤
因为后置为了返回旧值创建了⼀个临时对象，在函数结束的时候这个对象就会被销毁，函数不可以返回临时变量的引用！！！

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