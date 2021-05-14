// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem12.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=12 lang=cpp
 *
 * [12] 整数转罗马数字
 */

// @lc code=start
class Solution {
public:
    string intToRoman(int num) {

        vector<int> vec(4, 0);
        int pos = 3;
        while(num > 0){
            vec[pos--] = num % 10;
            num /= 10;
        }
        string res = "";
        if(vec[0] != 0){
            res += string(vec[0], 'M');
        }
        if(vec[1] != 0){
            if(vec[1] == 4){
                res += "CD";
            }else if(vec[1] == 9){
                res += "CM";
            }else if(vec[1] < 4){
                res += string(vec[1], 'C');
            }else{
                res += "D" + string(vec[1] - 5, 'C');
            }
        }
        if(vec[2] != 0){
            if(vec[2] == 4){
                res += "XL";
            }else if(vec[2] == 9){
                res += "XC";
            }else if(vec[2] < 4){
                res += string(vec[2], 'X');
            }else{
                res += "L" + string(vec[2] - 5, 'X');
            }
        }
        if(vec[3] != 0){
            if(vec[3] == 4){
                res += "IV";
            }else if(vec[3] == 9){
                res += "IX";
            }else if(vec[3] < 4){
                res += string(vec[3], 'I');
            }else{
                res += "V" + string(vec[3] - 5, 'I');
            }
        }
        return res;
    }
};
// @lc code=end

