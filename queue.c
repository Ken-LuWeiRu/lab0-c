#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}


/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *current, *temp;
    list_for_each_safe (current, temp, head) {
        element_t *entry = list_entry(current, element_t, list);
        list_del(current);
        free(entry->value);
        free(entry);
    }
    free(head);
}


/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *new_element = (element_t *) malloc(sizeof(element_t));
    if (new_element == NULL) {
        return false;
    }

    new_element->value = strdup(s);
    if (new_element->value == NULL) {
        free(new_element);
        return false;
    }

    list_add(&new_element->list, head);
    return true;
}



/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;
    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    list_add_tail(&new_element->list, head);
    return true;
}


/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *element = list_first_entry(head, element_t, list);
    list_del(&element->list);
    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}


/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *element = list_last_entry(head, element_t, list);
    list_del(&element->list);
    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int count = 0;
    struct list_head *iterator;
    list_for_each (iterator, head) {
        count++;
    }
    return count;  // Assuming one list_head represents one element
}

/* Delete the middle node in queue */
// https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return false;

    struct list_head *slow = head->next, *fast = head->next;
    struct list_head *prev = NULL;

    while (fast != head && fast->next != head) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    if (prev != NULL) {
        element_t *middle_element = list_entry(slow, element_t, list);
        list_del(slow);
        free(middle_element->value);
        free(middle_element);
        return true;
    }
    return false;
}

/* Delete all nodes that have duplicate string */
// https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
/* Delete all nodes that have duplicate string */
// because it is sored list, so we compare cuurent value with previous value
// until we find the different value, then we can delete the duplicate value
// because the duplicate point all have to remove, we need a flag to know
// wheather previous value need to remove
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;  // Return false if the list is empty or head is NULL.
    }

    struct list_head *current = head->next, *next_node;
    bool global_deleted = false;
    bool duplicate_flag = false;

    while (current != head && current->next != head) {
        element_t *current_element = list_entry(current, element_t, list);
        next_node = current->next;  // Move to the next node early because
                                    // current might get deleted.
        element_t *next_element = list_entry(next_node, element_t, list);

        // Check if current and next nodes have the same value.
        if (strcmp(current_element->value, next_element->value) == 0) {
            duplicate_flag = true;
            global_deleted = true;
        }

        // Move ahead to find the end of duplicates if there are any.
        while (duplicate_flag && next_node != head &&
               strcmp(current_element->value, next_element->value) == 0) {
            struct list_head *temp = next_node->next;
            list_del(next_node);
            free(next_element->value);
            free(next_element);
            next_node = temp;  // Proceed to next node.
            next_element = next_node != head
                               ? list_entry(next_node, element_t, list)
                               : NULL;
        }

        // If duplicates were found, delete the current node as well.
        if (duplicate_flag) {
            list_del(current);
            free(current_element->value);
            free(current_element);
            duplicate_flag = false;  // Reset the flag for the next iteration.
        }

        current =
            next_node;  // Proceed to next node for the next loop iteration.
    }

    return global_deleted;  // Return true if any element was deleted.
}


/* Swap every two adjacent nodes */
// https://leetcode.com/problems/swap-nodes-in-pairs/
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;  // No need to swap if the list is empty or has only one
                 // element.
    }

    struct list_head *current = head->next;  // Start with the first element.

    while (current != head && current->next != head) {
        struct list_head *next = current->next;    // The node to swap with.
        struct list_head *next_next = next->next;  // The node after the pair.

        // Detach current and next from the list.
        current->prev->next = next;
        next->next->prev = current;

        // Swap current and next.
        current->next = next_next;
        next->prev = current->prev;

        next->next = current;
        current->prev = next;

        // Move to the next pair.
        current = next_next;
    }
}

/* Reverse elements in queue */
// pass
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head *current = head->next;
    struct list_head *temp = NULL;
    struct list_head *prev = head;

    while (current != head) {
        temp = current->next;  // Save next node
        current->next = prev;  // Reverse current node's pointer
        prev = current;        // Move prev up for next iteration
        current = temp;        // Move to next node in original list
    }
    head->next = prev;  // Adjust head to new first element

    // Fix the previous links for each node
    current = head;
    do {
        temp = current->next;  // Move to next node
        temp->prev = current;  // Set the previous link
        current = temp;        // Move forward in the list
    } while (current != head);
}


/* Reverse the nodes of the list k at a time */
// https://leetcode.com/problems/reverse-nodes-in-k-group/
// i am goint to modify below code into our form
// struct ListNode* reverseKGroup(struct ListNode* head, int k) {
//     struct ListNode *curr = head;
//     int count = 0;

//     // First count k nodes
//     while (curr != NULL && count != k) {
//         curr = curr->next;
//         count++;
//     }

//     // If we counted k nodes, proceed with reversing them
//     if (count == k) {
//         // Reverse next k nodes and get the new head
//         curr = reverseKGroup(curr, k);

//         // Now head points to the kth node. So, reverse current k-group.
//         while (count > 0) {
//             struct ListNode *tmp = head->next; // temporary store for the
//             next node head->next = curr; // point the next of the current
//             node to the new head after reversal curr = head; // move the
//             current head to the next node in the original list head = tmp; //
//             move head to the next node in the original list count--;
//         }
//         head = curr; // reset head to the new reversed head after the entire
//         k-group is reversed
//     }
//     return head; // return the head of the modified list
// }
void q_reverseK(struct list_head *head, int k)
{
    if (head == NULL || list_empty(head) || k < 2) {
        return;  // No need to reverse if the list is empty, k is less than 2,
                 // or head is NULL.
    }

    struct list_head *group_prev =
        head;  // The node before the start of the group to be reversed
    struct list_head *group_first =
        head->next;  // The first node of the current group

    while (group_first != head) {
        struct list_head *current = group_first;
        struct list_head *group_end = current;
        int count = 1;

        // Find the end of the current group or end of the list
        while (count < k && group_end->next != head) {
            group_end = group_end->next;
            count++;
        }

        // Only reverse if we have k elements
        if (count == k) {
            struct list_head *next_group_first =
                group_end->next;  // The first node of the next group

            // Reverse the group
            struct list_head *prev = next_group_first;
            while (current != next_group_first) {
                struct list_head *next = current->next;
                current->next = prev;
                if (prev != head) {  // Avoid invalid memory access
                    prev->prev = current;
                }
                prev = current;
                current = next;
            }

            // Reattach the reversed group back to the list
            group_first->next = next_group_first;
            if (next_group_first != head) {  // Check for end of list
                next_group_first->prev = group_first;
            }
            group_prev->next = group_end;
            group_end->prev = group_prev;

            // Prepare for the next group
            group_prev = group_first;
            group_first = next_group_first;
        } else {
            // Not enough elements left to form a full group, so break out of
            // the loop
            break;
        }
    }
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head *sorted =
        NULL;  // This will be the start of the new sorted list
    struct list_head *current = head->next;  // Start from the first element

    while (current != head) {
        struct list_head *next = current->next;  // Store next for later

        // Remove 'current' from the list
        current->prev->next = current->next;
        current->next->prev = current->prev;

        // Find the location to insert the current node in the sorted part
        if (sorted == NULL ||
            (descend
                 ? strcmp(list_entry(sorted, element_t, list)->value,
                          list_entry(current, element_t, list)->value) < 0
                 : strcmp(list_entry(sorted, element_t, list)->value,
                          list_entry(current, element_t, list)->value) > 0)) {
            // Insert at the beginning
            current->next = sorted;
            if (sorted != NULL) {
                sorted->prev = current;
            }
            sorted = current;
            sorted->prev = NULL;  // Current node is now the first node
        } else {
            struct list_head *tmp = sorted;
            while (
                tmp->next != NULL &&
                (descend
                     ? strcmp(list_entry(tmp->next, element_t, list)->value,
                              list_entry(current, element_t, list)->value) > 0
                     : strcmp(list_entry(tmp->next, element_t, list)->value,
                              list_entry(current, element_t, list)->value) <
                           0)) {
                tmp = tmp->next;  // Move to next node
            }

            // Insert 'current' after 'tmp'
            current->next = tmp->next;
            if (tmp->next != NULL) {
                tmp->next->prev = current;
            }
            tmp->next = current;
            current->prev = tmp;
        }

        current = next;  // Move to the next node
    }

    // Reconnect the sorted list back to head
    if (sorted != NULL) {
        head->next = sorted;
        sorted->prev = head;

        // Find the last element and connect it back to head
        while (sorted->next != NULL) {
            sorted = sorted->next;
        }
        sorted->next = head;
        head->prev = sorted;
    }
}



/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
// https://leetcode.com/problems/remove-nodes-from-linked-list/
// LeetCode 2487. Remove Nodes From Linked List
// step 1: know the length of queue
// step 2: two pointer, cur and next, if  cur_val < value_next, then delete cur
// step 3: return the length of queue

int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;

    struct list_head *current = head->prev;  // 从链表末尾开始
    // 初始化一个临时变量来跟踪目前为止遇到的最小值
    char *min_value = list_entry(current, element_t, list)->value;
    int remain = 1;  // 用于记录保留下来的节点数量，末尾节点总是保留

    current = current->prev;  // 从倒数第二个节点开始检查

    while (current != head) {                    // 遍历直到回到头部
        struct list_head *prev = current->prev;  // 保存前一个节点以便后续遍历
        element_t *curr_elem = list_entry(current, element_t, list);

        // 如果当前节点的值大于目前为止遇到的最小值，则删除当前节点
        if (strcmp(curr_elem->value, min_value) > 0) {
            list_del(current);  // 从链表中删除当前节点
            // 释放节点占用的资源
            free(curr_elem->value);  // 释放字符串资源
            free(curr_elem);         // 释放节点资源
        } else {
            // 更新目前为止遇到的最小值并增加保留节点计数
            min_value = curr_elem->value;
            remain++;
        }

        current = prev;  // 移动到链表中的前一个节点
    }

    return remain;  // 返回链表中保留下来的节点数量
}


/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
// Plan 1: use two while loop to iterate every node
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;

    struct list_head *current = head->prev;  // 从链表末尾开始
    // 初始化一个临时变量来跟踪目前为止遇到的最大值
    char *max_value = list_entry(current, element_t, list)->value;
    int remain = 0;  // 用于记录保留下来的节点数量

    while (current != head) {                    // 遍历直到回到头部
        struct list_head *prev = current->prev;  // 保存前一个节点以便后续遍历
        element_t *curr_elem = list_entry(current, element_t, list);

        // 如果当前节点的值小于目前为止遇到的最大值，则删除当前节点
        if (strcmp(curr_elem->value, max_value) < 0) {
            list_del(current);  // 从链表中删除当前节点
            // 释放节点占用的资源
            free(curr_elem->value);  // 释放字符串资源
            free(curr_elem);         // 释放节点资源
        } else {
            // 更新目前为止遇到的最大值并增加保留节点计数
            max_value = curr_elem->value;
            remain++;
        }

        current = prev;  // 移动到链表中的前一个节点
    }

    return remain;  // 返回链表中保留下来的节点数量
}

// void q_free(struct list_head *head)
// {
//     if (!head)
//         return;
//     struct list_head *current, *temp;
//     list_for_each_safe (current, temp, head) {
//         element_t *entry = list_entry(current, element_t, list);
//         list_del(current);
//         free(entry->value);
//         free(entry);
//     }
//     free(head);
// }

/* Merge two sorted queues */
// step 1: merge two queues into one queue
// step 2: sort the merged queue
// do not using malloc!
void q_merge_two(struct list_head *head, struct list_head *head2, bool descend)
{
    if (!head || !head2) {
        return;  // Error case
    }

    list_splice_tail(head2, head);  // Merge the two queues
    INIT_LIST_HEAD(head2);
}


/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
// https://leetcode.com/problems/merge-k-sorted-lists/
// Function to merge all queues into one.
// step 1: merge all the queues into one queue, by function in lish.h
// step 2: sort the merged queue
// do not using malloc!
/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return 0;  // Error case
    }

    // Initialize a temporary pointer for safe iteration
    queue_contex_t *container, *temp_container;

    // We'll use the first queue as the base for merging.
    struct list_head *merged_queue =
        list_first_entry(head, queue_contex_t, chain)->q;

    // Iterate through all the queues linked from 'head', starting from the
    // second one.
    list_for_each_entry_safe (container, temp_container, head, chain) {
        // Skip the first queue as it's used as the base for merging.
        if (container->q != merged_queue) {
            q_merge_two(merged_queue, container->q, descend);
            // After merge, the container's queue is no longer needed, clear it
            // to avoid double freeing.
            INIT_LIST_HEAD(container->q);
        }
    }

    // Now all queues have been merged into 'merged_queue'.
    // Sort the merged queue.
    q_sort(merged_queue, descend);

    // Return the size of the merged queue. Assuming q_size is correctly
    // implemented.
    return q_size(merged_queue);
}
