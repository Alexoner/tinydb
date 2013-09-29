#include "str.h"
#include "database.h"
#include <stdio.h>

typedef struct row//每一个该结构体代表一行数据
{
    struct strs *column;//指向该列的数据
    struct row *last;
    struct row *next;
} row;
//函数返回值：0，错误；1，升序；2，降序
//OrderColumn为将要按照排序的ColumnName(要用二重指针)
//注意，使用完本函数后还要对返回的OrderColumn进行合理性判断（看是否出现在TargetColumn中）
int select_order(char *order,char **OrderColumn)
{
    if(order)
    {
        char orderColumnName[strlen(order)+1];
        char orderOption[strlen(order)+1];
        memset(orderColumnName,'\0',sizeof(orderColumnName));
        memset(orderOption,'\0',sizeof(orderOption));
        int or_i=0,or_cn_i=0,or_op_i=0;
        int first=1;
        for(or_i=0; or_i<strlen(order); or_i++)
        {
            if((order[or_i]>='a' && order[or_i]<='z') || (order[or_i]>='A' && order[or_i]<='Z') || order[or_i]==' '||order[or_i]=='_')
            {
                if(order[or_i]==' ')
                    first=2;
                else
                {
                    if(first==1)orderColumnName[or_cn_i++]=order[or_i];
                    if(first==2)orderOption[or_op_i++]=order[or_i];
                }
            }
            else
            {
                //printf("error!\n");
                return 0;
            }
        }
        if((strcmp(str_tolower(orderColumnName),"desc")==0 || strcmp(str_tolower(orderColumnName),"incr")==0) && strlen(orderOption)==0)
        {
            *OrderColumn=NULL;
            if(strcmp(str_tolower(orderColumnName),"desc")==0)return 2;
            else return 1;
        }
        if(strcmp(str_tolower(orderOption),"desc")!=0 && strcmp(str_tolower(orderOption),"incr")!=0)
        {
            //printf("error!\n");
            return 0;
        }
        else
        {
            *OrderColumn=(char *)malloc(sizeof(orderColumnName)+1);
            strcpy((*OrderColumn),orderColumnName);
            if(strcmp(str_tolower(orderOption),"desc")==0)return 2;
            else return 1;
        }


    }
    else
    {
        *OrderColumn=NULL;
        return 1;
    }
}
//函数返回值：0，错误；1，正常
//TargetColumn即为选出的column的column_name的集合(链表形式)
int select_column(Table *pt,char *column,strs **TargetColumn)
{
    int cl_i;
    int all=0;
    strs **TargetColumnP;
    TargetColumnP=TargetColumn;
    strs *ColumnName,**CNp;
    ColumnName=NULL;
    CNp=&ColumnName;//printf("column=%s\n",column);
    if(column==NULL)
    {
        //printf("error!(command of select is wrong.)\n");
        return 0;
    }
    if(column[0]=='*')
    {
        for(cl_i=1; cl_i<strlen(column); cl_i++)
            if(column[cl_i]!=' ')
            {
                //printf("error!(command of select is wrong.)\n");
                return 0;
            }
        Column *temp;
        temp=pt->head_column;
        while(temp)
        {
            (*TargetColumnP)=(strs *)malloc(sizeof(strs));
            (*TargetColumnP)->data=(char *)malloc(strlen(temp->column_name)+1);
            strcpy((*TargetColumnP)->data,temp->column_name);
            (*TargetColumnP)->next=NULL;
            (*TargetColumnP)->last=NULL;
            TargetColumnP=&(*TargetColumnP)->next;
            temp=temp->next;
        }
    }
    else
    {
        char name[strlen(column)+1];//考虑存在多个单词（column name）的情况！！！（后面必须+1，若只有一个单词，则会出现数组没法装下最后一个\0情况）
        memset(name,'\0',sizeof(name));
        int n_i=0;
        for(cl_i=0; cl_i<=strlen(column); cl_i++)
        {
            if((column[cl_i]>='a' && column[cl_i]<='z') || (column[cl_i]>='A' && column[cl_i]<='Z')||column[cl_i]=='_')
                name[n_i++]=column[cl_i];
            else if(column[cl_i]==','|| column[cl_i]=='\0')
            {
                 /*
                strs **TC_exist=TargetColumn;
                while(*TC_exist)//若用户select了两个一样的column的话，则报错返回
                {
                    if(strcmp((*TC_exist)->data,name)==0)return 0;
                    TC_exist=&(*TC_exist)->next;
                }
                */
                (*TargetColumnP)=(strs *)malloc(sizeof(strs));
                (*TargetColumnP)->data=(char *)malloc(strlen(name)+1);
                strcpy((*TargetColumnP)->data,name);
                (*TargetColumnP)->next=NULL;
                (*TargetColumnP)->last=NULL;
                TargetColumnP=&(*TargetColumnP)->next;
                n_i=0;
                memset(name,'\0',sizeof(name));
            }
            else
            {
                //printf("error!(command of select is wrong.)\n");
                return 0;
            }
        }//printf("\nname=%s\n",name);
    }
    return 1;
}
//column参数的要求是不能含有任何空格，几个单词之间用只能用逗号间隔，否则报错
char *Select(struct table *pt,char *column,char *term,char *order)
{
    strs *TargetColumn=NULL;//在经过下面的检验后，将储存被选出来的所有column_name，并且一定存在于table中
    int state=0;
    state=select_column(pt,column,&TargetColumn);
    if(state==0)return "error";
    //判断TargetColumn中的Column_name是否存在于table中
    strs *TCtest=TargetColumn;
    while(TCtest)
    {
        Column *temp=pt->head_column;
        int exist=0;
        while(temp)
        {
            if(strcmp(TCtest->data,temp->column_name)==0)
            {
                exist=1;
                break;
            }
            temp=temp->next;
        }
        if(exist==0)return "error";
        TCtest=TCtest->next;
    }
    //if(pt->head_column==NULL)printf("aaaa\n\n");
    if(TargetColumn==NULL)
    {
        //printf("test\n\n");
        return "$";//这是Table下的head_column为空时的特殊情况
    }



    int OrderTurn=1;//OrderTurn是排序规则，默认1为升序，然后2为降序
    char *OrderColumn=NULL;//OrderColumn是将要按照排序的那一列的名字
    OrderTurn=select_order(order,&OrderColumn);
    if(OrderTurn==0)return "error";
    if(OrderColumn)//当OrderColumn不为空时，检验其值是否存在于Table中
    {
        Column *OrderTest=pt->head_column;
        int OrderTestExist=0;
        while(OrderTest)
        {
            if(strcmp(OrderTest->column_name,OrderColumn)==0)
            {
                OrderTestExist=1;
                break;
            }
            OrderTest=OrderTest->next;
        }
        if(OrderTestExist==0)
        {
            return "$";
        }
    }

    nums *TermColumn=NULL;//储存的将是所有满足条件的列（也即Value链表中的节点位置）
    if(term==NULL)          //注意，若没有添加where语句，且table为空的时候，TermColumn将会为null
    {
        nums **p;
        p=&TermColumn;
        Value *temp;
        int index=1;
        temp=pt->head_column->head_value;
        while(temp)
        {
            *p=(nums *)malloc(sizeof(nums));
            (*p)->data=(int *)malloc(sizeof(int));
            (*p)->next=NULL;
            (*p)->data=index++;
            p=&(*p)->next;
            temp=temp->next;
        }
        if(TermColumn==NULL)return "$";//去掉最后一种特殊情况Table下head_column为空的时候。
    }
    else
    {
        //printf("pt=%s\tterm=%s\n",pt->table_name,term);
        TermColumn=solve_complex(pt,term);
        //printf("\nsuccessful!!!\n\n");
        if(TermColumn==NULL)//return "1error";
        {printf("error=%d\n",error);
            if(error==EMPTY)
            {
                error=ERROR;
                return "$";
            }
            else return "error";
        }
    }

/*
    nums *test=TermColumn;
    while(test)
    {
        printf("sssssss:%d\n",test->data);
        test=test->next;
    }
    if(TermColumn==NULL)printf("cacaca\n");*/
    /*
    strs *DotFloat=NULL;
    strs **DotFloatP=&DotFloat;
    strs *TCP=TargetColumn;
    while(TCP)
    {
        Column *find=pt->head_column;
        while(find)
        {
            if(strcmp(find->column_name,TCP->data)==0)break;
            find=find->next;
        }
        if(find->datatype==FLOAT)
        {
            *DotFloatP=(strs *)malloc(sizeof(strs));
            (*DotFloatP)->data=(char *)malloc(strlen(TCP->data)+1);
            strcpy((*DotFloatP)->data,TCP->data);
            (*DotFloatP)->last=NULL;
            (*DotFloatP)->next=NULL;
            DotFloatP=&(*DotFloatP)->next;
        }
        TCP=TCP->next;
    }
    */

    row *result=NULL;
    row **p;
    p=&result;
    nums *TermColumnP=TermColumn;
    while(TermColumnP)//value链表中的满足条件的节点.
    {
        int target=TermColumnP->data;//printf("target:%d\n",target);
        strs *TargetColumnP=TargetColumn;
        (*p)=(row *)malloc(sizeof(row));
        (*p)->last=NULL;
        (*p)->next=NULL;
        (*p)->column=NULL;
        strs **headp;
        headp=&(*p)->column;
        while(TargetColumnP)//将要被选择的列
        {
            Column *cp=pt->head_column;
            while(cp)
            {
                if(strcmp(cp->column_name,TargetColumnP->data)==0)break;
                cp=cp->next;
            }//printf("\t\t\t\t\t\t\tTCP=%s\n",TargetColumnP->data);
            /*
            int add=0;
            if(DotFloat)
                if(strcmp(DotFloat->data,TargetColumnP->data)==0)
                {
                    add=1;
                    DotFloat=DotFloat->next;
                }
            */

            Value *vp=cp->head_value;
            int index=0;
            while(vp)
            {//printf("vpvp=%s\tindex=%d\ttarget=%d\n",vp->data,index,target);
                index++;
                if(index==target)break;
                vp=vp->next;
            }//printf("test\tdata=%s\n",vp->data);

            char *change=(char *)malloc(strlen(vp->data)+1);
            memset(change,'\0',sizeof(change));
            strcpy(change,vp->data);

            if(change[0]=='\'')
            {
                int i=0,more=0;
            for(i=0;i<strlen(change);i++)
              if(change[i]==' ')
              {
                  more=1;
                  break;
              }
            if(more==1)
            {
                for(i=0;i<strlen(change);i++)
                if(change[i]=='\'')change[i]='"';
            }
            else
            {
                for(i=0;i<strlen(change)-1;i++)
                    change[i]=change[i+1];
                change[i-1]='\0';
            }
            }

            (*headp)=(strs *)malloc(sizeof(strs));
            (*headp)->last=NULL;
            (*headp)->next=NULL;
            (*headp)->data=(char *)malloc(strlen(vp->data)+1);
            //if(add==0)
            strcpy((*headp)->data,change);//printf("\t\t\taaa\n");
            free(change);
            /*
            else
            {
                char *tempData=NULL;
                tempData=(char *)malloc(strlen(vp->data)+5);
                memset(tempData,'\0',sizeof(tempData));
                strcpy(tempData,vp->data);

            }*/
            headp=&(*headp)->next;
            TargetColumnP=TargetColumnP->next;
        }
        p=&(*p)->next;
        TermColumnP=TermColumnP->next;
    }//printf("test\n\n");
    p=&result;
    while((*p)->next)//解决那个指向上一个链结的状况
    {
        (*p)->next->last=*p;
        p=&(*p)->next;
    }
/*
row *test1=result;
while(test1)
{
    strs *t=test1->column;
    while(t)
    {
        printf("testtest=%s\t",t->data);
        t=t->next;
    }
    printf("\n");
    test1=test1->next;
}
*/

    int OrderPosition=1;//按照在result链表中的每一行的第OrderPosition位置的value排序
    if(OrderColumn)
    {
        TCtest=TargetColumn;
        int pos=0;
        while(TCtest)
        {
            pos++;
            if(strcmp(TCtest->data,OrderColumn)==0)
            {
                OrderPosition=pos;
                break;
            }
            TCtest=TCtest->next;
        }
    }
    p=&result;
    while(*p)//将输出的值排序
    {
        row **pp=p;
        while(*pp)
        {
            strs *front=(*p)->column;
            int index=0;
            while(front)
            {
                index++;
                if(index==OrderPosition)break;
                front=front->next;
            }
            strs *back=(*pp)->column;
            index=0;
            while(back)
            {
                index++;
                if(index==OrderPosition)break;
                back=back->next;
            }
            if(OrderTurn==1)//升序
            {
                if(strcmp(front->data,back->data)>0)
                {
                    strs *copy=(*p)->column;
                    (*p)->column=(*pp)->column;
                    (*pp)->column=copy;
                }
            }
            else
            {
                if(strcmp(front->data,back->data)<0)
                {
                    strs *copy=(*p)->column;
                    (*p)->column=(*pp)->column;
                    (*pp)->column=copy;
                }
            }
            pp=&(*pp)->next;
        }
        p=&(*p)->next;
    }
    p=&result;
    while((*p)->next)p=&(*p)->next;
    while(*p)//若有#，将其移至开头
    {
        strs *pp=(*p)->column;
        int jing=0,handle=0;
        while(pp)
        {
            if(strcmp(pp->data,"#")==0)
            {
                jing=1;
                break;
            }
            pp=pp->next;
        }
        if(jing==1)
        {
            strs *copy1=(*p)->column;
            row **change=p;
            while((*change)->last)
            {
                (*change)->column=(*change)->last->column;
                change=&(*change)->last;
            }
            (*change)->column=copy1;
            handle=1;
        }
        if(handle==0)p=&(*p)->last;
        else
        {
            row *quit=*p;
            int Jall=1;
            while(quit)
            {
                strs *quit_test=quit->column;
                int Jexist=0;
                while(quit_test)
                {
                    if(strcmp(quit_test->data,"#")==0)
                    {
                        Jexist=1;
                        break;
                    }
                    quit_test=quit_test->next;
                }
                if(Jexist==0)
                {
                    Jall=0;break;
                }
                quit=quit->last;
            }
            if(Jall==1)break;
        }
    }
    char *Print;
    row *count=result;
    int tol=0;
    while(count)
    {
        strs *col=count->column;
        while(col)
        {
            tol+=strlen(col->data);
            tol++;
            col=col->next;
        }
        count=count->next;
    }
    Print=(char *)malloc(tol+2);
    Print[0]='\0';
    while(result)
    {
        strs *col=result->column;
        while(col)
        {
            strcat(Print,col->data);
            strcat(Print,",");
            col=col->next;
        }
        int i=0;
        while(1)
        {
            if(Print[i+1]=='\0')
            {
                Print[i]='\n';
                break;
            }
            i++;
        }
        result=result->next;
    }
    int i=0;
    while(1)
    {
        if(Print[i+1]=='\0')
        {
            Print[i]='\0';
            break;
        }
        i++;
    }
    return Print;
}
