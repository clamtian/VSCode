// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem725.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=725 lang=cpp
 *
 * [725] 分隔链表
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        int n = 0;
        ListNode *node = head;
        while(node) ++n, node = node->next;
        vector<ListNode*> res;
        node = head;
        while(k){
            int len = n % k && n > 0 ? n / k + 1 : n / k;
            ListNode *part = node;
            n -= len;
            while(len) part = part->next, --len;
            res.push_back(node);
            if(part){
                node = part->next;
                part->next = NULL;
            } 
            k -= 1;
        }
        return res;
    }
};
// @lc code=end

