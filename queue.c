#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"


queue_t *merge(queue_t *left, queue_t *right, queue_t *q);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->qsize = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *cur = q->head;
    list_ele_t *prev = NULL;
    while (cur) {
        prev = cur;
        cur = cur->next;
        free(prev->value);
        free(prev);
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
    if (q == NULL)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = (char *) malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s));
    (newh->value)[strlen(s)] = '\0';
    newh->next = q->head;

    q->head = newh;
    (q->qsize)++;
    if (q->qsize == 1)
        q->tail = newh;
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
    if (q == NULL)
        return false;
    list_ele_t *newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = (char *) malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s));
    (newh->value)[strlen(s)] = '\0';
    newh->next = NULL;

    if (q->tail != NULL)
        q->tail->next = newh;
    q->tail = newh;
    (q->qsize)++;

    if (q->qsize == 1)
        q->head = q->tail;

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
    if (q == NULL || q->qsize == 0)
        return false;
    if (sp != NULL) {
        memset(sp, '\0', bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    }
    list_ele_t *prev = q->head;
    q->head = q->head->next;

    free(prev->value);
    free(prev);
    (q->qsize)--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q != NULL)
        return q->qsize;
    return 0;
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
    if (q == NULL || q->qsize == 0)
        return;

    q->tail = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *next = NULL;
    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    q->head = prev;
}

//  compares two strings character by character.
static bool is_less_then(list_ele_t *x, list_ele_t *y)
{
    if (!x)
        return true;
    if (!y)
        return false;

    if (strcmp(x->value, y->value) < 0)
        return true;
    else
        return false;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->qsize < 2)
        return;

    queue_t left, right;
    left.qsize = q->qsize / 2 + (q->qsize & 1);
    right.qsize = q->qsize / 2;

    list_ele_t *cur = left.head = q->head;
    right.tail = q->tail;

    for (int i = 0; i < left.qsize - 1; i++)
        cur = cur->next;

    left.tail = cur;
    right.head = cur->next;
    left.tail->next = right.tail->next = NULL;
    q->head = q->tail = NULL;

    q_sort(&left);
    q_sort(&right);
    merge(&left, &right, q);
}

queue_t *merge(queue_t *left, queue_t *right, queue_t *q)
{
    q->qsize = left->qsize + right->qsize;
    list_ele_t *l = left->head, *r = right->head;
    list_ele_t *tmp = NULL;

    if (is_less_then(left->head, right->head))
        q->head = left->head;
    else
        q->head = right->head;

    q->tail = q->head;
    for (int i = 0; i < q->qsize; i++) {
        if (!r || (l && is_less_then(l, r))) {
            tmp = l;
            l = l->next;
        } else {
            tmp = r;
            r = r->next;
        }
        q->tail->next = tmp;
        q->tail = tmp;
    }
    tmp->next = NULL;
    return q;
}