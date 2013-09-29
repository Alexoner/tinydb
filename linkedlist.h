/*************************************************************************
    > File Name: linkedlist.h
    > Author: hao
    > Created Time: Thu 18 Oct 2012 10:53:02 AM CST
 ************************************************************************/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SORT_NUM 1
#define SORT_STR 2

struct linkedlist
{
    void *data;
    struct linkedlist *next;
};

/*head is the head pointer or its address of a linkedlist,and
 * next_offset is the offset of the menber "next" pointer in the struct,
 * which points to the next struct int the linkedlist
 */

void *ListInit(void **head,size_t next_offset);

void *ListDestroy(void **head,size_t next_offset);

int ListEmpty(void *head,size_t next_offset);//whether empty

int ListLength(void *head,size_t next_offset);
//void *GetNode(void *head,size_t next_offset);

void *ListLocateNode(void *head,void *p,
        int (*compare)(void *,void *),size_t next_offset);
//find firest node matched  "compare" with struct pointer p

void *ListTail(void *head,size_t next_offset);//locate the last node

void *ListPriorNode(void *head,void *p,size_t next_offset);
//find the predecessor of p

void *ListNextNode(void *head,void *p,size_t next_offset);
//find successor of element p

void *ListInsertBefore(void **head,void *position,void *q,size_t next_offset);
//insert an element q before p

void *ListInsertAfter(void **head,void *position,void *q,size_t next_offset);
//insert q after p

void *ListPrepend(void **head,void *p,size_t next_offset);
//prepend an element p to the list

void *ListAppend(void **head,void *p,size_t next_offset);
//append an element p to the list

void *ListDeleteNode(void **head,void *p,size_t next_offset);
//delete element p

void ListTraverse(void *head,int (*visit)(void*),
        size_t next_offset);

void *ListOrderIncr(void *head,size_t size,
        size_t data_offset,size_t next_offset,int data_flag);
//sort a list by increasing order,string or number,
//returning the head pointer of the list

void *ListOrderDesc(void *head,size_t size,size_t data_offset,
        size_t next_offset,int data_flag);
//same as ListOrderDecr,by descending order.

int ListSave(void *head,size_t size,
        size_t next_offset,const char*path);

int ListLoad(void **head,size_t size,size_t next_offset,
        const char*path);
#endif
