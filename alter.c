/*************************************************************************
    > File Name: alter.c
    > Created Time: Mon 22 Oct 2012 02:29:45 PM CST
 ************************************************************************/
#include "linkedlist.h"
#include "database.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


char type[][9]={"int","float","text"};//datatype

char *alter_table(struct table *pt,int option,char *argument)
{
    struct column *pc=NULL,*qc=NULL;
    //pc is to get string from argument
    struct value *pv=NULL;
    char *str=NULL;
    int i,length=0;
    size_t offset=offsetof(Column,next);
    if(!pt||!argument)
        return 0;
    str=(char*)malloc(strlen(argument)+1);
    pc=(struct column*)malloc(sizeof(struct column));
    memset(pc,0,sizeof(Column));
    pc->db_name=pt->db_name;
    pc->table_name=pt->table_name;
    switch(option)
    {
        case ADD:
            //validate legality
            i=sscanf(argument,"%[a-zA-Z_]",str);
            if(!i)
            {//new column name
                free(pc);
                free(str);
                return "error";
            }
            printf("40:%s\n",str);
            length=strlen(str);
            pc->column_name=str;
            qc=(Column*)ListLocateNode(pt->head_column,pc,column_equal,offsetof(Column,next));
            if(qc)
            {//previous column with the same name exists
                free(pc);
                free(str);
                return "error";
            }

            if(strchar(str+strlen(str))[0]!='\0')
            {
                i=sscanf(argument+length,"%s",str);
                if(!i)
                {//get the datatype
                    free(pc);
                    free(str);
                    return "error";
                }
                if(strcmp(str_tolower(str),type[0])&&
                        strcmp(str_tolower(str),type[1])&&strcmp(str_tolower(str),type[2]))
                {//none datatype matches
                    free(pc);
                    free(str);
                    return "error";
                }//validate the input legality
            }

            i=sscanf(argument,"%[a-zA-Z_]",str);
            //printf("34:\n");
            length=strlen(str);
            pc->column_name=(char*)malloc(strlen(str)+1);
            strcpy(pc->column_name,str);
            qc=(Column*)ListLocateNode(pt->head_column,pc,column_equal,offsetof(Column,next));
            if(qc)
            {
                free(pc);
                return "error";
            }

            //length+=getnword(argument+length,str,1);
            if(i=sscanf(argument+length,"%s",str))
            {
            if(!strcmp(str_tolower(str),"int"))
                pc->datatype=INT;
            else if(!strcmp(str_tolower(str),"float"))
                pc->datatype=FLOAT;
            else if(!strcmp(str_tolower(str),"text"))
                pc->datatype=TEXT;
            }
            else
                pc->datatype=NONE;
            ListAppend((void**)&pt->head_column,pc,offset);
            length=ListLength(pt->head_column->head_value,offsetof(Value,next));
            for(i=0;i<length;i++)
            {//make up for the missing values
                pv=(Value*)malloc(sizeof(Value));
                memset(pv,0,sizeof(Value));
                pv->data=(char*)malloc(strlen("#")+1);
                strcpy(pv->data,"#");
                ListAppend((void**)&pc->head_value,pv,offsetof(Value,next));
            }
            break;
        case DROP:
            length+=getnword(argument+length,str,1);
            pc->column_name=(char*)malloc(strlen(str));
            strcpy(pc->column_name,str);
            //printf("drop column:%s\n",pc->column_name);
            qc=ListLocateNode(pt->head_column,pc,column_equal,offset);
            if(qc)
            {
                ListDeleteNode((void**)&pt->head_column,qc,offset);
            }
            free(pc);
            break;
        case ALTER_COLUMN:
            if(length+=getnword(argument+length,str,1))
            {//get the column name
                pc->column_name=(char*)malloc(strlen(str));
                strcpy(pc->column_name,str);
                qc=ListLocateNode(pt->head_column,pc,column_equal,offset);
                if(qc)
                {
                    length+=getnword(argument+length,str,1);
                    if(strcmp(str,type[0])&&strcmp(str,type[1])&&strcmp(str,type[2]))
                    {
                        //printf("alter.c: error\n");
                    }
                    else if(!strcmp(str_tolower(str),"int"))
                        pc->datatype=INT;
                    else if(!strcmp(str_tolower(str),"float"))
                        pc->datatype=FLOAT;
                    else if(!strcmp(str_tolower(str),"text"))
                    {
                        if(pc->datatype==INT||pc->datatype==FLOAT)
                        {
                            free(pc);
                            return "error";
                        }
                        pc->datatype=TEXT;
                    }
                    else
                    {
                        free(pc);
                        return "error";
                    }
                }
                else
                {
                    free(pc);
                    return "error";
                }
            }
            free(pc);
            break;
            default:;
    }
    return NULL;
}




