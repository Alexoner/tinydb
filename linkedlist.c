/*************************************************************************
    > File Name: linkedlist.c
    > Author: hao
    > Created Time: Wed 17 Oct 2012 11:41:43 PM CST
 ************************************************************************/
#include "linkedlist.h"
#include <stdlib.h>
#include <stddef.h>

void *ListInit(void**head,size_t next_offset)
{
    if(!*head)
        ListDestroy(head,next_offset);
    *head=NULL;
    return *head;
}

void *ListDestroy(void **head,size_t next_offset)
{
    void *p,*q;
    for(p=*head;p;p=q)
    {
        memcpy(&q,p+next_offset,sizeof(void*));
        free(p);
    }
    *head=NULL;
    return *head;
}

int ListEmpty(void *head,size_t next_offset)
{
    if(!head)
        return 1;
    return (*(void**)head==NULL);
}

int ListLength(void *head,size_t next_offset)
{
    int i;
    void *p=NULL;
    for(i=0,p=head;p;memcpy(&p,p+next_offset,sizeof(void*)),i++);
    return i;
}

void *ListLocateNode(void *head,void *p,int (*compare)(void*,void*),
        size_t next_offset)
{
    void *q;
    for(q=head;q&&!compare(p,q);memcpy(&q,q+next_offset,sizeof(void*)));
    return q;
}

void *ListTail(void *head,size_t next_offset)
{
    void *p;
    for(p=head;*(void**)(p+next_offset);memcpy(&p,p+next_offset,sizeof(void*)));
    return p;
}

void *ListPriorNode(void *head,void *p,size_t next_offset)
{
    void *q;
    for(q=head;q&&*(void**)(q+next_offset)!=p;memcpy(&q,q+next_offset,sizeof(void*)));
    return q;
}

void *ListNextNode(void *head,void *p,size_t next_offset)
{
    void *q;
    for(q=head;q&&*(void**)(p+next_offset)!=q;memcpy(&q,q+next_offset,sizeof(void*)));
    return q;
}

void *ListInsertBefore(void **head,void *position,void *p,size_t next_offset)
{
    void *q=NULL;
    q=ListPriorNode(*head,position,next_offset);
    if(q)
    {
        memcpy(p+next_offset,q+next_offset,sizeof(void*));
        memcpy(q+next_offset,&p,sizeof(void*));
        return p;
    }
    else if(*head==p)
    {
        memcpy(p+next_offset,(*head+next_offset),sizeof(void*));
        memcpy(head,&p,sizeof(void*));
        return p;
    }
    else
        return  NULL;
}


void *ListInsertAfter(void **head,void *position,void *q,size_t next_offset)
{//insert q after p
    if(!*head)
    {//empty list
        memcpy(head,&q,sizeof(void*));
        memset(q+next_offset,0,sizeof(void*));
        return q;
    }
    else if(position)
    {
        memcpy(q+next_offset,position+next_offset,sizeof(void*));
        memcpy(position+next_offset,&q,sizeof(void*));
    }
    else
        return NULL;
    return q;
}

void *ListPrepend(void **head,void *p,size_t next_offset)
{
    if(!*head)
        *head=p;
    else
    {
        memcpy(p+next_offset,head,sizeof(void*));
        memcpy(head,&p,sizeof(void*));
    }
    return p;
}

void *ListAppend(void **head,void *p,size_t next_offset)
{
    void *q=ListPriorNode(*head,NULL,next_offset);//locate the last node
    if(q)
    {
        memset(p+next_offset,0,sizeof(void*));
        memcpy(q+next_offset,&p,sizeof(void*));
    }
    else if(!*head)
    {
        ListInsertAfter(head,*head,p,next_offset);
    }
    return p;
}


void *ListDeleteNode(void **head,void *position,size_t next_offset)
{
    void *q;
    if(*head==position)
    {//delete the first node
        q=position;
        memcpy(head,position+next_offset,sizeof(void*));
        free(q);
    }
    else
    {
        q=ListPriorNode(*head,position,next_offset);
        if(!q)
            return NULL;//node not found
        memcpy(q+next_offset,position+next_offset,sizeof(void*));
        free(position);
    }
    return q;
}

void ListTraverse(void *head,int (*visit)(void*),size_t next_offset)
{
    void *p;
    for(p=head;p;memcpy(&p,p+next_offset,sizeof(void*)))
        visit(p);
}

void *ListOrderIncr(void *head,size_t size,
        size_t data_offset,size_t next_offset,int data_flag)
{
    void *ptr=malloc(size),*p=NULL,*q=NULL;
    int i,j,alter,n=ListLength(head,next_offset);
    for(i=0;i<n;i++)
        for(j=0,p=head;p&&j<n-1;j++)
        {
            memcpy(&q,p+next_offset,sizeof(void*));//q is after p
            if(data_flag==SORT_NUM)
            {
                alter=(*(double*)(p+data_offset)>*(double*)(q+data_offset));
            }
            else if(data_flag==SORT_STR)
            {
                alter=(strcmp(*(char**)(p+data_offset),*(char**)(q+data_offset))>0);
            }
            if(alter)
            {
                memcpy(ptr,p,size);//hold p
                memcpy(p,q,size);
                memcpy(p+next_offset,&q,sizeof(void*));//the next pointer
                memcpy(ptr+next_offset,q+next_offset,sizeof(void*));
                memcpy(q,ptr,size);
            }
            memcpy(&p,p+next_offset,sizeof(void*));
        }
    free(ptr);
    return head;
}

void *ListOrderDesc(void *head,size_t size,
        size_t data_offset,size_t next_offset,int data_flag)
{
    void *ptr=malloc(size),*p=NULL,*q=NULL;
    int i,j,alter,n=ListLength(head,next_offset);
    for(i=0;i<n;i++)
        for(j=0,p=head;p&&j<n-1;j++)
        {
            memcpy(&q,p+next_offset,sizeof(void*));//q is after p
            if(data_flag==SORT_NUM)
            {
                alter=(*(double*)(p+data_offset)<*(double*)(q+data_offset));
            }
            else if(data_flag==SORT_STR)
            {
                alter=(strcmp(*(char**)(p+data_offset),*(char**)(q+data_offset))<0);
            }
            if(alter)
            {
                memcpy(ptr,p,size);//hold p
                memcpy(p,q,size);
                memcpy(p+next_offset,&q,sizeof(void*));//the next pointer
                memcpy(ptr+next_offset,q+next_offset,sizeof(void*));
                memcpy(q,ptr,size);
            }
            memcpy(&p,p+next_offset,sizeof(void*));
        }
    free(ptr);
    return head;
}


int ListSave(void *head,size_t size,size_t next_offset,const char *path)
{
    void *p;
    FILE *fp=fopen(path,"wb");
    if(!fp)
    {
        perror("open file to save");
        return 0;
    }
    for(p=head;p;memcpy(&p,p+next_offset,sizeof(void*)))
    {
        fwrite(p,size,1,fp);
        printf("121: %d\n",*(int*)p);
    }
    fclose(fp);
    return 1;
}

int ListLoad(void **head,size_t size,size_t next_offset,const char *path)
{
    void *p=malloc(size),*q=NULL;
    if(!p)
    {
        perror("LoadList\n");
        return 0;
    }
    FILE *fp=fopen(path,"rb");
    if(!fp)
    {
        perror("In linkedlist.c,LoadList");
        return 0;
    }
    while(!feof(fp))
    {
        fread(p,size,1,fp);
        if(!feof(fp))
        {
            memset(p+next_offset,0,sizeof(void*));
            ListAppend(head,p,next_offset);
        }
        printf("149\n");
    }
    fclose(fp);
    return 1;
}


