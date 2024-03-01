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
    INIT_LIST_HEAD(
        head);  
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
    if (!head || !s)
        return false;
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;
    new_element->value =
        strdup(s);  // strdup allocates memory and copies the string into it
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    list_add(&new_element->list,
             head);  
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
    list_add_tail(&new_element->list,
                  head);  
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
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;  
    }

    struct list_head *current = head->next, *temp;  
    while (current != head && current->next != head) { 
        element_t *current_entry = list_entry(current, element_t, list);
        element_t *next_entry = list_entry(current->next, element_t, list);

        if (strcmp(current_entry->value, next_entry->value) == 0) {  
            temp = current->next; 
            list_del(current->next);  
            free(next_entry->value); 
            free(next_entry);  
            current = temp;
        } else {
            current = current->next;  
        }
    }
}

/* Swap every two adjacent nodes */
// https://leetcode.com/problems/swap-nodes-in-pairs/
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return; 
    }

    struct list_head *current = head->next; 

    while (current != head && current->next != head) { 
        struct list_head *next = current->next;
        struct list_head *nextnext = next->next;

        next->next = current;
        current->next = nextnext;
        if (nextnext != head) { 
            nextnext->prev = current;
        }

        if (current == head->next) { 
            head->next = next;
        }

        current = nextnext;
    }


    struct list_head *temp = head;
    do {
        temp->next->prev = temp;
        temp = temp->next;
    } while (temp != head);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head) {
    if (!head || list_empty(head)) return 0;

    struct list_head *current = head, *temp;
    int removed = 0; 

    struct list_head new_head;
    INIT_LIST_HEAD(&new_head);

    char *max_value = NULL;
    element_t *current_entry;

    while (current->next != head) {
        current = current->next; 
        current_entry = list_entry(current, element_t, list);


        if (!max_value || strcmp(current_entry->value, max_value) >= 0) {
            max_value = current_entry->value;
            list_move_tail(current, &new_head); 
        } else {
            temp = current->prev; 
            list_del(current); 
            free(current_entry->value);
            free(current_entry); 
            current = temp; 
            removed++;
        }
    }

    if (!list_empty(&new_head)) {
        head->next = new_head.next;
        new_head.next->prev = head;
        head->prev = new_head.prev;
        new_head.prev->next = head;
    }

    return removed; 
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
