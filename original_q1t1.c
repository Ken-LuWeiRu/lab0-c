#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // 加入 time.h 以使用 clock()

typedef struct __node {
    struct __node *left, *right;
    struct __node *next;
    long value;
} node_t;


void list_add(node_t **list, node_t *node_t)
{
    node_t->next = *list;
    *list = node_t;
}

node_t *list_tail(node_t **left)
{
    while ((*left) && (*left)->next)
        left = &((*left)->next);
    return *left;
}

int list_length(node_t **left)
{
    int n = 0;
    while (*left) {
        ++n;
        left = &((*left)->next);
    }
    return n;
}

node_t *list_construct(node_t *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = list;
    node->value = n;
    return node;
}

void list_free(node_t **list)
{
    node_t *node = (*list)->next;
    while (*list) {
        free(*list);
        *list = node;
        if (node)
            node = node->next;
    }
}

// void quick_sort(node_t **list)
// {
//     int n = list_length(list);
//     int value;
//     int i = 0;
//     int max_level = 2 * n;
//     node_t *begin[max_level], *end[max_level];
//     node_t *result = NULL, *left = NULL, *right = NULL;

//     begin[0] = *list;
//     end[0] = list_tail(list);

//     while (i >= 0) {
//         node_t *L = begin[i], *R = end[i];
//         if (L != R) {
//             node_t *pivot = L;
//             value = pivot->value;
//             node_t *p = pivot->next;
//             pivot->next = NULL;

//             while (p) {
//                 node_t *n = p;
//                 p = p->next;
//                 list_add(n->value > value ? &right : &left, n);
//             }

//             begin[i] = left;
//             end[i] = list_tail(&left);
//             begin[i + 1] = pivot;
//             end[i + 1] = pivot;
//             begin[i + 2] = right;
//             end[i + 2] = list_tail(&right);

//             left = right = NULL;
//             i += 2;
//         } else {
//             if (L)
//                 list_add(&result, L);
//             i--;
//         }
//     }
//     *list = result;
// }
void quick_sort(node_t **list)
{
    int length =
        list_length(list);  // 更改這裡的變量名，避免與下方的 node_t 變量重名
    int value;
    int i = 0;
    int max_level = 2 * length;  // 這裡使用修改後的變量名
    node_t *begin[max_level], *end[max_level];
    node_t *result = NULL, *left = NULL, *right = NULL;

    begin[0] = *list;
    end[0] = list_tail(list);

    while (i >= 0) {
        node_t *L = begin[i], *R = end[i];
        if (L != R) {
            node_t *pivot = L;
            value = pivot->value;
            node_t *p = pivot->next;
            pivot->next = NULL;

            while (p) {
                node_t *node_ptr = p;  // 修改變量名稱，避免重名
                p = p->next;
                list_add(node_ptr->value > value ? &right : &left,
                         node_ptr);  // 這裡也使用修改後的變量名
            }

            begin[i] = left;
            end[i] = list_tail(&left);
            begin[i + 1] = pivot;
            end[i + 1] = pivot;
            begin[i + 2] = right;
            end[i + 2] = list_tail(&right);

            left = right = NULL;
            i += 2;
        } else {
            if (L)
                list_add(&result, L);
            i--;
        }
    }
    *list = result;
}


/* Verify if list is order */
static bool list_is_ordered(node_t *list)
{
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    // if (n <= 0)
    //     return;
    // 移除這行，因為 size_t 類型的 n 永遠不會小於零
    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}



int main(int argc, char **argv)
{
    node_t *list = NULL;
    const int sizes[] = {10000, 50000, 100000};  // 定義不同大小的測試案例
    const int num_sizes =
        sizeof(sizes) / sizeof(sizes[0]);  // 計算測試案例的數量
    double elapsed_times[num_sizes];  // 儲存每個測試案例的執行時間

    for (int s = 0; s < num_sizes; ++s) {
        size_t count = sizes[s];
        int *test_arr = malloc(sizeof(int) * count);

        // 產生隨機數據
        for (int i = 0; i < count; ++i)
            test_arr[i] = i;
        shuffle(test_arr, count);

        // 構建鏈表
        for (size_t i = 0; i < count; ++i)
            list = list_construct(list, test_arr[i]);

        clock_t start = clock();  // 開始計時
        quick_sort(&list);        // 執行快速排序
        clock_t end = clock();    // 結束計時

        elapsed_times[s] =
            (double) (end - start) / CLOCKS_PER_SEC;  // 計算經過時間

        assert(list_is_ordered(list));  // 驗證列表是否已排序
        list_free(&list);               // 釋放列表記憶體
        free(test_arr);                 // 釋放測試陣列記憶體
    }

    // 輸出每個測試案例的執行時間
    for (int i = 0; i < num_sizes; ++i) {
        printf("Size %d: %f seconds\n", sizes[i], elapsed_times[i]);
    }

    return 0;
}
