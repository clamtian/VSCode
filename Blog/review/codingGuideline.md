# 1.while判断语句永远不要写k--和--k

```javascript
while (k--) {  // 跳出循环体后,k变成了-1
    doSomething();
}
```