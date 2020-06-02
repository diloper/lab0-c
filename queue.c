#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

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
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (!q)
        return;

    list_ele_t *tmp = q->head;
    while (q->head) {
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
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
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    newh->next = q->head;
    q->head = newh;

    if (!q->tail)
        q->tail = newh;
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
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));
    newh->next = NULL;

    if (!q->tail) {
        q->head = q->tail = newh;
    } else {
        q->tail->next = newh;
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
    if (!q || !q->head)
        return false;

    if (sp) {
        size_t realbufsize = (bufsize > strlen(q->head->value))
                                 ? strlen(q->head->value)
                                 : bufsize - 1;
        memset(sp, '\0', realbufsize + 1);
        strncpy(sp, q->head->value, realbufsize);
    }

    list_ele_t *tmp;

    tmp = q->head;
    q->head = q->head->next;
    //    tmp->next = NULL;
    free(tmp->value);
    free(tmp);

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return !q ? 0 : q->size;
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
    if (!q || q->size < 2)
        return;

    list_ele_t *tmp;
    tmp = q->head->next;
    q->tail->next = q->head;

    while (tmp != q->tail) {
        tmp = tmp->next;
        q->head->next->next = q->tail->next;
        q->tail->next = q->head->next;
        q->head->next = tmp;
    }
    q->tail = q->head;
    q->head = q->head->next;
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->head == q->tail)
        return;
    q->head = q_merge_sort(q, q->head);
    /* Method 1 :Update q->tail
     */

    //    while (q->tail->next) {
    //        q->tail = q->tail->next;
    //    }
}
list_ele_t *q_merge_sort(queue_t *q, list_ele_t *head)
{
    if (!head)
        return NULL;
    if (!head->next)
        return head;

    list_ele_t *right = head->next;
    list_ele_t *left = head;

    // --split--
    while (right && right->next) {
        left = left->next;
        right = right->next->next;
    }
    right = left->next;
    left->next = NULL;

    // --recursive--
    list_ele_t *l1 = q_merge_sort(q, head);
    list_ele_t *l2 = q_merge_sort(q, right);

    //-- merge --
    list_ele_t merge;
    list_ele_t *list = &merge;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) <= 0) {
            list->next = l1;
            l1 = l1->next;
        } else {
            list->next = l2;
            l2 = l2->next;
        }
        list = list->next;
    }

    list->next = l1 ? l1 : l2;
    /* Method 2:Update q->tail
     */
    for (; list; list = list->next)
        q->tail = list;
    return merge.next;
}
