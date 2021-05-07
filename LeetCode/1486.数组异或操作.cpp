/*
 * @lc app=leetcode.cn id=1486 lang=cpp
 *
 * [1486] 数组异或操作
 */

// @lc code=start
class Solution {
public:
    int xorOperation(int n, int start) {
        int s = start;
        if(start % 4 == 2 || start % 4 == 3){
            switch(n % 4){
                case 0 : return s ^ 2 ^ (s + 2 * n - 2);
                    break;
                case 1 : return s;
                    break;
                case 2 : return s ^ (s + 2 * n - 2);
                    break;
                case 3 : return s ^ 2;
                    break;
            }
        }else{
            switch(n % 4){
                case 0 : return 0;
                    break;
                case 1 : return s + 2 * n - 2;
                    break;
                case 2 : return 2;
                    break;
                case 3 : return 2 ^ (s + 2 * n - 2);
                    break;
            }
        }
        return 0;
    }
};
// @lc code=end

