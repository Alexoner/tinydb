/*************************************************************************
    > File Name: database.c
    > Created Time: Sat 20 Oct 2012 05:00:03 PM CST
 ************************************************************************/
#include "database.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include "calc.h"

int error=ERROR;

char * create_db(struct db **pd, char* dname)
{
    struct db *ptr,*q=NULL;
    size_t offset=offsetof(struct db,next);

    strtrim(dname);
    dname=strchar(dname);
    if(match_key(dname))
        return "error";
    ptr=(struct db*)malloc(sizeof(struct db));
    memset(ptr,0,sizeof(Db));
    ptr->name=dname;
    q=(Db*)ListLocateNode(*pd,ptr,db_equal,offsetof(Db,next));
    if(q)
    {
        free(ptr);
        return "error";
    }
    else
    {
        ptr->name=(char*)malloc(strlen(dname)+1);
        strcpy(ptr->name,dname);
    }
    ListAppend((void**)pd,ptr,offset);
    return NULL;
}

char * create_table(struct db *pd, char* table_name, char* column)
{
    struct table *pt=NULL,*qt=NULL;
    size_t offset=offsetof(Table,next);
    char *str,*str1,*buf;

    if(match_key(table_name)||!is_text(table_name))
        return "error";

    //judge whether name exsits previously
    pt=(Table*)malloc(sizeof(Table));
    memset(pt,0,sizeof(Table));
    str1=strtrim(table_name);
    str1=strchar(str1);
    pt->table_name=str1;
    qt=(Table*)ListLocateNode(pd->head_table,pt,table_equal,offsetof(Table,next));
    if(qt)
    {
        free(str1);
        free(pt);
        return "error";
    }
    free(pt);
    pt=NULL;
    //judge whether name exists


    //printf("65:%s\n",column);
    if(column)
    {
        str=strchar(column);
        strtrim(str);
        if(!(str[0]=='('&&str[strlen(str)-1]==')'))
            return "error";
        str=strbtw(column,'(',')');
        //printf("71:%s\n",column);
        if(!isscsv(column,WITH_NUM))
        {
            //printf("74:wrong format for columns\n");
            return "error";
        }

        //str=strchar(str+1);
        strtrim(str);
        if(match_key(str1))
            return "error";

        /*buf=(char*)malloc(strlen(column)+1);
        while(1)
        {
            str1=str;
            if(!(i=sscanf(str1,"%[a-zA-Z_]",buf)))
                return "error";
            str1+=strlen(buf);
            str1=strchar(buf);
            i=sscanf(str1,"%[a-zA-Z]",buf);
            if(!i)
            {
                if(str1[0]!=',')
                    return "error";
                else if(str1[0]=='\0')
                    break;
            }
            str1+=strlen(buf);
            str1=strchar(str1);
            if(str1[0]==',')
                str1=strchar(str1+1);
            if(str1[0]=='\0')
                break;
        }//check syntax errors for column
        free(buf);
        str1=buf=NULL;*/
    }

    pt=(struct table*)malloc(sizeof(struct table));
    memset(pt,0,sizeof(Table));
    pt->db_name=pd->name;
    pt->table_name=(char*)malloc(strlen(str1)+1);
    strcpy(pt->table_name,str1);

    //qt=ListPriorNode(pd->head_table,NULL,offset);
    ListAppend((void**)&pd->head_table,pt,offset);
    if(column)
    {
        str=strchar(str);
        buf=(char*)malloc(strlen(column)+1);
        while(str<str+strlen(str))
        {
            //printf("123:%s\n",str);
            sscanf(str,"%[^,]",buf);
            str+=strlen(buf);
            if(str[0]==',')
                str++;
            str=strchar(str);
            create_column(pt,buf);
        }
    }
    return NULL;
}

char *create_column(struct table *pt, char *column)
{
    struct column *pc=NULL,*qc;
    char *str=NULL,*buf=NULL;
    buf=(char*)malloc(strlen(column)+1);
    strtrim(column);
    str=strchar(column);
    sscanf(str,"%[^ ]",buf);
    pc=(struct column*)malloc(sizeof(struct column));
    memset(pc,0,sizeof(Column));
    pc->column_name=(char*)malloc(strlen(buf)+1);
    strcpy(pc->column_name,buf);
    //printf("70:%s\n",buf);
    pc->db_name=pt->db_name;
    pc->table_name=pt->table_name;
    qc=(Column*)ListLocateNode(pt->head_column,pc,column_equal,
            offsetof(Column,next));
    if(qc)
    {
        free(pc->column_name);
        free(pc);
        free(buf);
        return "error";
    }

    str+=strlen(buf);
    str=strchar(str);
    sscanf(str,"%[a-zA-Z]",buf);
    //printf("120:%s %s\n",str,buf);
    if(!strcmp(str_tolower(buf),"int"))
    {
        //printf("123,int\n");
        pc->datatype=INT;
    }
    else if(!strcmp(str_tolower(str),"float"))
        pc->datatype=FLOAT;
    else if(!strcmp(str_tolower(str),"text"))
        pc->datatype=TEXT;
    else
    {
        pc->datatype=NONE;
    }
    ListAppend((void**)&pt->head_column,pc,offsetof(Column,next));
    return NULL;
}

char * truncate_table(struct table *pt)
{
    struct column *pc=NULL;
    for(pc=pt->head_column;pc;pc=pc->next)
        ListDestroy((void**)&pc->head_value,offsetof(Value,next));
    ListDeleteNode((void**)&current_db->head_table,pt,offsetof(Table,next));
    return NULL;
}

char * use_database(struct db *hd,char *name)
{
    struct db *pd=NULL,*qd=NULL;
    pd=(struct db*)malloc(sizeof(struct db));
    pd->head_table=NULL;
    pd->name=(char*)malloc(strlen(name));
    strcpy(pd->name,name);
    pd->next=NULL;
    qd=ListLocateNode(head_db,pd,db_equal,offsetof(Db,next));
    current_db=qd;
    free(pd);
    return qd?NULL:"error";
}

char * drop_database(struct db **hd,char *name)
{
    struct db *pd=NULL,*qd=NULL;
    struct table *pt;
    struct column *pc;
    qd=(Db*)malloc(sizeof(Db));
    memset(qd,0,sizeof(Db));
    name=strchar(name);
    qd->name=str_tolower(name);
    pd=(struct db*)ListLocateNode(*hd,qd,db_equal,offsetof(Db,next));
    if(!pd)
    {
        free(qd);
        return "error";
    }
    for(pt=pd->head_table;pt;pt=pt->next)
    {
        for(pc=pt->head_column;pc;pc=pc->next);
        {
            ListDestroy((void**)&pc->head_value,offsetof(Value,next));
        }
        ListDestroy((void**)&pt->head_column,offsetof(Column,next));
    }
    ListDestroy((void**)&pd->head_table,offsetof(Table,next));
    ListDeleteNode((void**)&head_db,pd,offsetof(Db,next));
    free(qd);
    return NULL;
}

char *drop_table(struct db *hd,char *db_name,char *table_name)
{
    struct db *pd=NULL,*qd=NULL;
    struct table *pt=NULL,*qt=NULL;
    struct column *pc=NULL;
    qd=(Db*)malloc(sizeof(Db));
    memset(qd,0,sizeof(Db));
    qd->name=db_name;
    pd=(Db*)ListLocateNode(hd,qd,db_equal,offsetof(Db,next));
    if(!pd)
    {
        free(qd);
        return "error";
    }
    qt=(Table*)malloc(sizeof(Table));
    memset(qt,0,sizeof(qt));
    qt->table_name=table_name;
    pt=(Table*)ListLocateNode(pd->head_table,qt,table_equal,offsetof(Table,next));
    if(!pt)
    {

        free(qt);
        return "error";
    }
    for(pc=pt->head_column;pc;pc=pc->next)
        ListDestroy((void**)&pc->head_value,offsetof(Column,next));
    ListDeleteNode((void**)&pd->head_table,pt,offsetof(Table,next));
    free(qt);
    return NULL;
}

char * rename_table(struct table *ht,char *oldname,char *newname)
{
    struct table *pt=NULL,*qt=NULL;
    qt=(Table *)malloc(sizeof(Table));
    memset(qt,0,sizeof(qt));
    qt->table_name=oldname;
    pt=(Table*)ListLocateNode(ht,qt,table_equal,offsetof(Table,next));
    if(!pt)
    {
        free(qt);
        return "error";
    }
    pt->table_name=(char*)realloc(pt->table_name,strlen(newname));
    strcpy(pt->table_name,newname);
    free(qt);
    return NULL;
}

char * rename_database(struct db* hd,char *oldname,char *newname)
{
    struct db*pd=NULL,*qd=NULL;
    qd=(Db*)malloc(sizeof(Db));
    memset(qd,0,sizeof(qd));
    qd->name=oldname;
    pd=(Db*)ListLocateNode(hd,qd,db_equal,offsetof(Db,next));
    if(!pd)
    {
        free(qd);
        return "error";
    }
    pd->name=(char*)realloc(pd->name,strlen(newname));
    strcpy(pd->name,newname);
    free(qd);
    return NULL;
}


char *insert_into(struct table*ht,char *table_name,
        char *column_name,char *value)
{
    struct table *pt=NULL,*qt=NULL;
    struct column *pc=NULL,*qc=NULL;
    struct value *pv;
    int i=0,new_length=0,j;
    int clen,vnum,legality=1;
    float tmp=0.0;
    //column list length,column_name number,value number

    size_t offset=offsetof(Value,next);
    char *cbuf1,*cbuf2,*vbuf1,*vbuf2,*tstr,*cstr,*vstr;//column buffer,value buffer,.
    tstr=strchar(table_name);
    strtrim(tstr);
    pt=(Table*)malloc(sizeof(Table));
    memset(pt,0,sizeof(Table));
    pt->table_name=str_tolower(tstr);
    qt=(Table*)ListLocateNode(ht,pt,table_equal,offsetof(Table,next));
    if(!qt)
    {
        free(pt);
        return "error";
    }

    free(pt->table_name);
    free(pt);

    clen=ListLength(qt->head_column,offsetof(Column,next));
    //column length

    vbuf1=(char*)malloc(strlen(value)+1);//buffer for whole value string
    vbuf2=(char*)malloc(strlen(value)+1);//buffer for each value
    strcpy(vbuf1,value);
    strtrim(vbuf1);
    vstr=strchar(vbuf1);

    //validate input legality
    vnum=iscsv(vstr,WITH_NUM);
    for(i=1;vstr[i]!='\0'&&vstr[i]!=')';i++)
    {
        if((vstr[i]!=',')&&(vstr[i]!=' '))
            break;
    }
    //printf("348:%s %c\n",vstr,vstr[i]);
    if(vstr[i]=='\0'||vstr[i]==')')
    {
        free(vbuf1);
        free(vbuf2);
        return "error";
    }//experssion with no value

    //printf("361:%d\n",vnum);
    if(!vnum||vstr[0]!='('||vnum>clen)
    {
        if(vstr[0]!='(')
        {
            i=sscanf(vstr,"%[a-zA-Z_\'0-9]",vbuf2);
            vstr+=strlen(vbuf2);
            vstr=strchar(vstr);
            if(!(i&&vstr[0]=='\0'))
            {
                free(vbuf1);
                free(vbuf2);
                return "error";
            }
        }
        else
        {
            free(vbuf1);
            free(vbuf2);
            return "error";
        }
    }

    if(!column_name)
    {//validate values
        vstr=strchar(vbuf1);
        if(vstr[0]=='(')
            vstr=strchar(vstr+1);
        for(pc=qt->head_column;pc;pc=pc->next)
        {
            //printf("328:column name:%s\n",pc->column_name);
            if(vstr[0]=='\''||vstr[0]=='\"')
            {
                strquote(vstr,vbuf2);
                i=1;
            }
            else
            {
                i=sscanf(vstr,"%[^,) ]",vbuf2);//get a new value
            }
            //printf("387:%s\n",vbuf2);
            if(i)
            {
                if(pc->datatype==TEXT)
                {
                    //printf("325\n");
                    if(vbuf2[0]!='\''&&vbuf2[strlen(vbuf2)-1]!='\"')
                    {
                        legality=0;
                        break;//text data type expect ''
                    }
                }
                else if(pc->datatype==INT)
                {
                    if(!isint(vbuf2))
                    {
                        legality=0;
                        break;
                    }
                }
                else if(pc->datatype==FLOAT)
                {
                    if(!isfloat(vbuf2))
                    {
                        legality=0;
                        break;
                    }
                }
                else if(pc->datatype==NONE)
                {
                    legality=0;
                    break;
                }
            }//if i!=0
            vstr+=strlen(vbuf2);
            vstr=strchr(vstr,',');
            if(!vstr)
            {//comma not found
                legality=1;
                break;
            }
            else
            {
                vstr=strchar(vstr);
            }
            memset(vbuf2,0,strlen(vbuf2));
        }
    }
    else
    {//column specified,validate
        vstr=strchar(vbuf1);
        if(vstr[0]=='(')
            vstr=strchar(vstr+1);
        cbuf1=(char*)malloc(strlen(column_name)+1);
        strcpy(cbuf1,column_name);
        strtrim(cbuf1);
        cstr=strchar(cbuf1);
        cbuf2=(char*)malloc(strlen(cstr)+1);

        cstr=strchar(cstr+1);
        //cstr[strlen(cstr)-1]='\0';//get rid of last ')'

        while(1)
        {
            if(!(i=sscanf(cstr,"%[^, )]",cbuf2)))
            {
                legality=0;
                break;
            }//retrieve column name
            pc=(Column*)malloc(sizeof(Column));
            memset(pc,0,sizeof(Column));
            pc->column_name=cbuf2;
            qc=(Column*)ListLocateNode(qt->head_column,pc,
                    column_equal,offsetof(Column,next));//locate column
            if(!qc)
            {//column not found
                legality=0;
                break;
            }

            cstr+=strlen(cbuf2);
            memset(cbuf2,0,strlen(cbuf2));
            cstr=strchr(cstr,',');
            if(cstr)
                cstr=strchar(cstr+1);

            if(vstr[0]=='\''||vstr[0]=='\"')
            {
                strquote(vstr,vbuf2);
                i=1;
            }
            else
            {
                i=sscanf(vstr,"%[^,) ]",vbuf2);
            }
            //printf("346:%s datatype:%d vbuf2:%s\n",qc->column_name,qc->datatype,vbuf2);

            if(i)
            {
                if(qc->datatype==TEXT)
                {
                    if(vbuf2[0]!='\''&&vbuf2[strlen(vbuf2)-1]!='\"')
                    {
                        legality=0;
                        break;
                    }
                }
                if(qc->datatype==INT)
                {
                    if(!isint(vbuf2))
                    {
                        //printf("322:%s\n",cbuf2);
                        legality=0;
                        break;
                    }
                }
                else if(qc->datatype==FLOAT)
                {
                    if(!isfloat(vbuf2))
                    {
                        legality=0;
                        break;
                    }
                }
                else
                {
                    legality=0;
                    break;
                }
            }
            //printf("516:%s %s\n",qc->column_name,vbuf2);
            vstr+=strlen(vbuf2);
            memset(vbuf2,0,strlen(vbuf2));
            vstr=strchr(vstr,',');
            if(!vstr)
            {//check whether the string is finished
                legality=1;
                break;//over
            }
            else
            {
                vstr=strchar(vstr+1);
            }
        }
    }//column specified,finished validating input contents
    if(legality==0)
    {
        //printf("514:illegal!\n");
        free(vbuf1);
        free(vbuf2);
        if(column_name)
        {
            free(cbuf1);
            free(cbuf2);
        }
        return "error";
    }

    //after validating,execute the linked list operations
    if(!column_name)
    {//operate linked list
        vstr=strchar(vbuf1);
        if(vstr[0]=='(')
            vstr=strchar(vstr+1);
        memset(vbuf2,0,strlen(vbuf2));
        for(pc=qt->head_column;pc;pc=pc->next)
        {
            //printf("328:column name:%s\n",pc->column_name);
            if(vstr[0]=='\''||vstr[0]=='\"')
            {
                strquote(vstr,vbuf2);
                for(j=0;vbuf2[j]!='\0';j++)
                    if(vbuf2[j]=='\"')
                        vbuf2[j]='\'';
                //printf("576:%s\n",vbuf2);
                i=1;
            }
            else
            {
                i=sscanf(vstr,"%[^,) ]",vbuf2);//get a new value
            }
            pv=(struct value*)malloc(sizeof(struct value));
            memset(pv,0,sizeof(Value));
            //printf("568:%s %s i=%d\n",pc->column_name,vbuf2,i);
            if(i)
            {
                pv->data=(char *)malloc(strlen(vbuf2)+1);
                if(pc->datatype==FLOAT)
                {
                    tmp=atof(vbuf2);
                    sprintf(pv->data,"%.2f",tmp);
                }
                else
                {
                    strcpy(pv->data,vbuf2);
                }
            }
            else
            {
                pv->data=(char*)malloc(strlen("#")+1);
                strcpy(pv->data,"#");
            }
            ListAppend((void**)&pc->head_value,pv,offset);
            vstr+=strlen(vbuf2);
            memset(vbuf2,0,strlen(vbuf2));
            vstr=strchr(vstr,',');
            if(vstr)
                vstr=strchar(vstr+1);
        }
    }
    else
    {//column specified,operate linked list
        //cbuf1=(char*)malloc(strlen(column_name)+1);
        //strcpy(cbuf1,column_name);
        //strtrim(cbuf1);
        cstr=strchar(cbuf1);
        cbuf2=(char*)malloc(strlen(cstr)+1);

        cstr=strchar(cstr+1);
        //cstr[strlen(cstr)-1]='\0';//get rid of last ')'
        //printf("602:%s\n",cstr);
        vstr=strchar(vbuf1);
        if(vstr[0]=='(')
            vstr=strchar(vstr+1);

        while(1)
        {
            if(!(i=sscanf(cstr,"%[^, )]",cbuf2)))
            {
                break;
            }//retrieve column name
            pc=(Column*)malloc(sizeof(Column));
            memset(pc,0,sizeof(Column));
            pc->column_name=cbuf2;
            qc=(Column*)ListLocateNode(qt->head_column,pc,
                    column_equal,offsetof(Column,next));//locate column
            if(!qc)
            {
                return "error";
            }
            cstr+=strlen(cbuf2);
            memset(cbuf2,0,strlen(cbuf2));
            cstr=strchar(cstr);//column located
            if(cstr[0]==',')
                cstr=strchar(cstr+1);

            if(vstr[0]=='\''||vstr[0]=='\"')
            {
                strquote(vstr,vbuf2);
                for(j=0;vbuf2[j]!='\0';j++)
                    if(vbuf2[j]=='\"')
                        vbuf2[j]='\'';
                   i=1;
            }
            else
            {
                i=sscanf(vstr,"%[^,) ]",vbuf2);
            }
            //printf("346:%s datatype:%d vbuf2:%s\n",cbuf2,qc->datatype,vbuf2);


            pv=(struct value*)malloc(sizeof(struct value));
            memset(pv,0,sizeof(Value));
            if(i)
            {
                pv->data=(char *)malloc(strlen(vbuf2)+1);
                strcpy(pv->data,vbuf2);
            }
            else
            {
                pv->data=(char*)malloc(strlen("#")+1);
                strcpy(pv->data,"#");
            }//value retrieved

            ListAppend((void**)&qc->head_value,pv,offset);
            new_length=ListLength(qc->head_value,offset);
            //printf("516:%s %s\n",qc->column_name,((Value*)ListTail(qc->head_value,offsetof(Value,next)))->data);
            vstr+=strlen(vbuf2);
            memset(vbuf2,0,strlen(vbuf2));
            vstr=strchr(vstr,',');
            if(!vstr)
                break;//over
            else if(vstr[0]==',')
                vstr=strchar(vstr+1);
        }
    }//column specified

    for(pc=qt->head_column;pc;pc=pc->next)
    {//make up for the missing rows in other columns
        if(ListLength(pc->head_value,offsetof(Value,next))<new_length)
        {
            pv=(Value*)malloc(sizeof(Value));
            memset(pv,0,sizeof(Value));
            pv->data=(char*)malloc(2);
            strcpy(pv->data,"#");
            ListAppend((void**)&pc->head_value,pv,offsetof(Value,next));
        }
    }
    return NULL;
}



char * show_db(struct db *hd,int order)
{
    struct db *pd=hd;
    struct string
    {
        char *str;
        struct string *next;
    };
    size_t offset1=offsetof(struct string,str);
    size_t offset2=offsetof(struct string,next);
    struct string *hs=NULL,*ps=NULL;
    char *str=NULL;
    for(pd=hd;pd;pd=pd->next)
    {
        ps=(struct string*)malloc(sizeof(struct string));
        memset(ps,0,sizeof(struct string));
        ps->str=pd->name;
        ListAppend((void**)&hs,ps,offset2);
    }
    if(order==INCR)
        ListOrderIncr(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    else if(order==DESC)
        ListOrderDesc(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    if(!hs)
    {
        return "$";
    }
    str=(char*)malloc(strlen(hs->str)+1);
    strcpy(str,hs->str);
    for(ps=hs->next;ps;ps=ps->next)
    {
        str=(char*)realloc(str,strlen(str)+strlen(ps->str)+2);
        strcat(str,",");
        strcat(str,ps->str);
    }
    ListDestroy((void**)&hs,offset2);
    return str;
}

char * show_table(struct table *ht,int order)
{
    struct table *pt=ht;
    char *str=NULL;
    struct string
    {
        char *str;
        struct string *next;
    };
    size_t offset1=offsetof(struct string,str);
    size_t offset2=offsetof(struct string,next);
    struct string *hs=NULL,*ps=NULL;
    for(pt=ht;pt;pt=pt->next)
    {
        ps=(struct string*)malloc(sizeof(struct string));
        memset(ps,0,sizeof(struct string));
        ps->str=pt->table_name;
        ListAppend((void**)&hs,ps,offset2);
    }
    if(order==INCR)
        ListOrderIncr(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    else if(order==DESC)
        ListOrderDesc(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    if(!hs)
    {
        return "$";
    }
    str=(char*)malloc(strlen(hs->str)+1);
    strcpy(str,hs->str);
    for(ps=hs->next;ps;ps=ps->next)
    {
        str=(char*)realloc(str,strlen(str)+strlen(ps->str)+2);
        strcat(str,",");
        strcat(str,ps->str);
    }
    ListDestroy((void**)&hs,offset2);
    return str;
}

char *show_column(struct column *hc,int order)
{
    struct column *pc=hc;
    char *str=NULL;
    struct string
    {
        char *str;
        struct string *next;
    };
    size_t offset1=offsetof(struct string,str);
    size_t offset2=offsetof(struct string,next);
    struct string *hs=NULL,*ps=NULL;
    for(pc=hc;pc;pc=pc->next)
    {
        ps=(struct string*)malloc(sizeof(struct string));
        memset(ps,0,sizeof(struct string));
        ps->str=pc->column_name;
        ListAppend((void**)&hs,ps,offset2);
    }
    if(order==INCR)
        ListOrderIncr(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    else if(order==DESC)
        ListOrderDesc(hs,sizeof(struct string),offset1,offset2,SORT_STR);
    if(!hs)
    {
        return "$";
    }
    str=(char*)malloc(strlen(hs->str)+1);
    strcpy(str,hs->str);
    for(ps=hs->next;ps;ps=ps->next)
    {
        str=(char*)realloc(str,strlen(str)+strlen(ps->str)+2);
        strcat(str,",");
        strcat(str,ps->str);
    }
    ListDestroy((void**)&hs,offset2);
    return str;
}

int display_db_node(void *pd)
{
    //printf("%s\t",((struct db*)pd)->name);
    return 0;
}

int display_table_node(void *pt)
{
    //printf("%s\t",((struct table*)pt)->table_name);
    return 0;
}

int display_column_node(void *pc)
{
    //printf("%s %d,",((struct column*)pc)->column_name,((Column*)pc)->datatype);
    return 0;
}

int db_equal(void *p,void*q)
{
    return !strcmp(str_tolower(((Db*)p)->name),
            str_tolower(((Db*)q)->name));
}

int table_equal(void *p,void *q)
{
    return !strcmp(str_tolower(((Table*)p)->table_name),
            str_tolower(((Table*)q)->table_name));
}

int column_equal(void *p,void *q)
{
    return !strcmp(str_tolower(((Column*)p)->column_name),
            str_tolower(((Column*)q)->column_name));
}

nums *solve_complex(struct table *pt,char *expression)
{//return linkedlist's head pointer
    char *str1=NULL,*optr=NULL,*opnd=NULL,*buf=NULL;
    int i,n=0,j,k;
    struct expression *he=NULL,*pe=NULL;
    optr=(char*)malloc(strlen(expression));
    buf=(char*)malloc(strlen(expression));
    strcpy(buf,expression);
    buf=strtrim(buf);
    str1=strchar(buf);
    //printf("320:%p %s\n",str1,str1);
    while(str1[0]=='(')
    {//get rid of the outermost parenthesis
        opnd=strbtw(str1,'(',')');
        //printf("%ld %ld\n",strlen(opnd),strlen(str1));
        if(strlen(opnd)==strlen(str1)-2)
        {//a parenthesis around the expression,skip it
            //printf("748:%s\n",str1);
            free(opnd);
            str1++;
            buf[strlen(buf)-1]='\0';
            buf=strtrim(buf);
        }
        else//a parenthesis around the firest "single" expression
        {
            free(opnd);
            break;
        }
    }

    while(1)//str1 starts with '('
    {//logical algorithm
        //printf("760:%s\n",str1);
        if(str1[0]=='(')
        {//a complex expression
            opnd=strbtw(str1,'(',')');
            pe=(struct expression*)malloc(sizeof(struct expression));
            memset(pe,0,sizeof(struct expression));
            pe->type=OPND;
            pe->opnd=solve_complex(pt,opnd);
            //recursively call the function to deal with parenthesis

            he=(struct expression*)realloc(he,sizeof(struct expression)*(n+1));
            n++;
            *(he+n-1)=*pe;

            //ListAppend((void**)&he,pe,offsetof(struct expression,next));
            //printf("348,operand:%s\n",opnd);
            str1+=strlen(opnd)+2;
        }
        else
        {//current expression is a single expression
            opnd=streqt(str1);//get a equation
            if(!opnd)
            {
                free(optr);
                free(buf);
                return NULL;
            }//syntax error
            if(!strpbrk(opnd,"~=<>"))
            {//maybe it's "like" and "between" operation
                for(j=0,k=0;str1[j]!='\0'&&str1[j]!=']';j++,k++)
                {
                    opnd[k]=str1[j];
                }
                if(str1[j]==']')
                    opnd[k++]=']';
                opnd[k]='\0';
            }
            //printf("729:%s\n",opnd);

            pe=(struct expression*)malloc(sizeof(struct expression));
            memset(pe,0,sizeof(struct expression));
            pe->type=OPND;
            pe->opnd=solve_single(pt,opnd);
            he=(struct expression*)realloc(he,sizeof(struct expression)*(n+1));
            n++;
            *(he+n-1)=*pe;
            free(pe);
            //printf("368:set is %p %d\n",he->opnd,he->opnd->data);
            str1+=strlen(opnd)+1;
       }

        str1=strchar(str1);//skip white spaces

        if(str1<buf+strlen(buf))
        {//if it's a complex expression,there are some operators
            sscanf(str1,"%[a-zA-Z]",optr);//get the operator
            str1+=strlen(optr);
            str1=strchar(str1);
            pe=(struct expression*)malloc(sizeof(struct expression));
            memset(pe,0,sizeof(struct expression));
            pe->type=OPTR;
            //printf("821,optr:%s\n",optr);
            if(!strcmp(str_tolower(optr),"and"))
            {
                    //printf("595,optr:%s\n",optr);
                    pe->optr=AND;
            }
            else if(!strcmp(str_tolower(optr),"or"))
            {
                //printf("601:%s\n",optr);
                    pe->optr=OR;
            }
            else
                return NULL;
           he=(struct expression*)realloc(he,sizeof(struct expression)*(n+1));
           n++;
           *(he+n-1)=*pe;
        }
        else//finished
        {
            for(i=0;i<n;i++)
            {
                //printf("394:%d\t",(he+i)->optr);
            }
            return solve_expression(he,n);
            //return NULL;
        }
    }
    return NULL;
}



nums *solve_single(struct table *pt,char *expression)
{//judge whether
    char *cname,*optr,*value,*str;//optr:=,<,>
    struct column *pc=NULL,*qc=NULL;
    nums *ps=NULL;
    int i=0;
    str=strchar(expression);
    strtrim(str);
    if(str[0]=='(')
       return solve_complex(pt,expression);
    cname=(char*)malloc(strlen(str));
    optr=(char*)malloc(strlen(str));
    value=(char*)malloc(strlen(str));

    //printf("800:%s\n",expression);

    sscanf(str,"%[^=><~ ]",cname);
    str+=strlen(cname);
    str=strchar(str);
    i=sscanf(str,"%[~=><]",optr);
    if(!i)
    {
        i=sscanf(str,"%[a-zA-Z]",optr);
        if(!i||(!strcmp(str_tolower(optr),"like")&&
                    !strcmp(str_tolower(optr),"between")))
        {
            free(cname);
            free(optr);
            free(value);
            return NULL;
        }
    }

    str+=strlen(optr);
    str=strchar(str);
    strcpy(value,str);//maybe it's embeded value
    /*sscanf(str,"%*[^=><]%[=><]",optr);
    sscanf(str,"%*[^=><]%*[=><]%[^;]",value);*/
    qc=(Column*)malloc(sizeof(Column));
    memset(qc,0,sizeof(qc));
    qc->column_name=cname;
    //printf("997:%s,%s,%s\n",cname,optr,value);
    pc=ListLocateNode(pt->head_column,qc,column_equal,offsetof(Column,next));
    //printf("661:%s\n",pc->column_name);
    if(!pc)
    {
        return NULL;
    }
    if(pc->datatype==INT||pc->datatype==FLOAT)
    {
        //printf("672\n");
        return compute_num(pc,optr,value);
    }
    else if(pc->datatype==TEXT)
    {
        printf("1036:%s\n",value);
        ps=compute_str(pc,optr,value);
        return ps;
    }
    else if(pc->datatype==NONE)
            return NULL;
    else
        return NULL;
}



nums *solve_expression(struct expression *he,int n)
{
    struct expression *pe=NULL,stack1[n],stack2[n];
    int i,top1=-1,top2=-1;
    //stack1=(struct expression*)malloc(sizeof(struct expression)*n);
    //stack2=(struct expression*)malloc(sizeof(struct expression)*n);
    for(pe=he;pe<he+n;pe++)
    {
        if(pe->type==OPND)
        {
            stack1[++top1]=*pe;
        }
        else
        {//operator
            while(top2>=0)
            {//pop out of stack
                if(pe->optr<=stack2[top2].optr)
                {//priority of operator in stack isn't lower
                    stack1[++top1]=stack2[top2--];//pop
                }
                else
                    break;
            }
            stack2[++top2]=*pe;//current operator into stack
        }
    }
    while(top2+1)
    {
        stack1[++top1]=stack2[top2--];
    }

    for(i=0;i<n;i++)
    {
        if((stack1+i)->type==OPND)//stack1
            stack2[++top2]=*(stack1+i);//opnd into stack
        else
        {//operator
            if(top2<1)//error input
                return NULL;
            if(stack1[i].optr==AND)//watch out for memory leaks
            {
                stack2[top2-1].opnd=num_and(stack2[top2-1].opnd,stack2[top2].opnd);
            }
            else if(stack1[i].optr==OR)
            {
                stack2[top2-1].opnd=num_or(stack2[top2-1].opnd,stack2[top2].opnd);
            }
            top2--;
        }
    }
    if(top2)
        return NULL;
    return (stack2+top2)->opnd;
}


nums *compute_num(struct column *pc,char *optr,char *right_value)
{
    nums *hn=NULL,*pn=NULL;
    struct value *pv=NULL;
    int i=0,match=0;
    double result1,result2;
    char *str=NULL,*command=NULL,*lower=NULL,*higher=NULL;
    lower=(char*)malloc(strlen(right_value)+1);
    higher=(char*)malloc(strlen(right_value)+1);
    if(!(strcmp(str_tolower(optr),"between")))
    {
        i=sscanf(right_value,"%*[^0-9]%[0-9.]",lower);
        //printf("998:%s %s %s\n",optr,right_value,lower);
        if(!i)
        {
            free(lower);
            free(higher);
            return NULL;
        }
        str=strchr(right_value,',');
        i=sscanf(right_value,"%*[^,],%[0-9.]",higher);
        if(!i)
        {
            free(lower);
            free(higher);
            return NULL;
        }
        //printf("%s\n",higher);
        result1=atof(lower);
        result2=atof(higher);
        for(i=0,pv=pc->head_value;pv;pv=pv->next,i++)
        {
            if(atof(pv->data)>result1&&atof(pv->data)<result2)
            {
                pn=(nums*)malloc(sizeof(nums));
                memset(pn,0,sizeof(nums));
                pn->data=i+1;
                ListAppend((void**)&hn,pn,offsetof(nums,next));
            }
        }
        if(!hn)
        {
            error=EMPTY;//print "$",not "error"
        }
        return hn;
    }


    else if(ismexp(right_value))//right_value is number expression
    {
        //printf("987 column:%s %s %s %f\n",pc->column_name,optr,right_value,result1);
        //buf=(char*)malloc(strlen(right_value)+2);
        //strcat(buf,"\0");
        if(!calc_num(right_value,&result1))
            return NULL;
        for(i=0,pv=pc->head_value;pv;pv=pv->next,i++)
        {
            match=comp_num(atof(pv->data),result1,optr);
            //printf("737:math expression %d\n",comp_num(atof(pv->data),result,optr));
            if(match)
            {//match the relationship indicated by optr
                //printf("737:%s\n",pv->data);
                pn=(nums*)malloc(sizeof(nums));
                memset(pn,0,sizeof(nums));
                pn->data=i+1;
                ListAppend((void**)&hn,pn,offsetof(nums,next));
            }
        }
        if(!hn)
        {
            error=EMPTY;
        }
        return hn;
    }
    else
    {
        str=strchar(right_value);
        if(str[0]=='(')
        {
            if(str[strlen(str)-1]!=')')
                return NULL;
            command=strbtw(str,'(',')');
        }
        else
            command=str;
        command=(char*)realloc(command,strlen(command)+2);
        strcat(command,";");
        //printf("867:%s\n",command);
        str=str_adjust(command);
        //printf("869:%s\n",str);
        if(!isfloat(str))
            return NULL;
        return compute_num(pc,optr,str);
    }
}



