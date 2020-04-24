#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

// define private function
list_ele_t *sort(list_ele_t *start);

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }

    while (q->head) {
        list_ele_t *temp = q->head;
        q->head = q->head->next;
        temp->next = NULL;
        free(temp->value);
        free(temp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!q || !newh) {
        return false;
    }

    int len = strlen(s) + 1;
    newh->value = malloc(sizeof(char) * len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', len);
    strncpy(newh->value, s, len);

    if (!q->tail) {
        q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!q || !newh) {
        return false;
    }
    newh->next = NULL;

    int len = strlen(s) + 1;
    newh->value = malloc(len);
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', len);
    strncpy(newh->value, s, len);

    if (q->tail) {
        q->tail->next = newh;
        q->tail = newh;
    } else {
        q->head = newh;
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    list_ele_t *temp = q->head;
    if (sp) {
        memset(sp, '\0', strlen(sp) + 1);
        strncpy(sp, temp->value, bufsize - 1);
    }
    q->head = q->head->next;
    temp->next = NULL;
    free(temp->value);
    free(temp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q) {
        return;
    }

    q->tail = q->head;
    list_ele_t *reverse = NULL;
    list_ele_t *cursor = q->head;
    while (cursor) {
        list_ele_t *temp = cursor;
        cursor = cursor->next;
        temp->next = reverse;
        reverse = temp;
    }
    q->head = reverse;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    q->head = sort(q->head);
}

list_ele_t *sort(list_ele_t *start)
{
    if (!start || !start->next) {
        return start;
    }
    list_ele_t *left = start;
    list_ele_t *right = left->next;
    left->next = NULL;

    left = sort(left);
    right = sort(right);

    for (list_ele_t *merge = NULL; left || right;) {
        if (!right || (left && strcmp(left->value, right->value) < 0)) {
            if (!merge) {
                start = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        } else {
            if (!merge) {
                start = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return start;
}
