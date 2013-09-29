/*************************************************************************
    > File Name: main.c
    > Created Time: Sat 20 Oct 2012 07:12:45 PM CST
 ************************************************************************/
#include "linkedlist.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct db *head_db=NULL,*current_db=NULL;

int main(int argc,char **argv)
{
    int i=0;
    char ch,*input=NULL,*str;
    FILE *fpin,*fpout;
    struct db *pd=NULL,*qd;
    //struct column *pc;
    if(argc)
    {
        fpin=fopen(*(argv+1),"r");
        if(!fpin)
        {
            //perror("open file");
            fpin=stdin;
        }
    }
    else
        fpin=stdin;
    if(argc>=2)
    {
        fpout=fopen(*(argv+2),"w");
        if(!fpout)
            fpout=stdout;
    }
    else
        fpout=stdout;
    /*printf("seed_cup$ ");
    create_db(&head_db,"world");
    create_db(&head_db,"hello");
    create_db(&head_db,"abi");
    create_db(&head_db,"seedcup");
    qd=(struct db*)malloc(sizeof(struct db));
    qd->head_table=NULL;
    qd->name="world";
    qd->next=NULL;
    pd=ListLocateNode((void*)head_db,qd,db_equal,offsetof(struct db,next));
    current_db=pd;
    create_table(pd,"build","(age int)");
    create_table(pd,"book","(size int,area float,category text,number int)");
    create_table(pd,"teacher","(sex int,height int,hobby text)");
    create_table(pd,"river",NULL);

    insert_into(pd->head_table,"build",NULL,"(12)");
    insert_into(pd->head_table,"book",NULL,"(15,0.1,'man  ga',1)");
    insert_into(pd->head_table,"book",NULL,"(12,0.2,'math',2)");
    insert_into(pd->head_table,"book",NULL,"(21,0.1,'manga',3)");*/

    while((ch=fgetc(fpin))!=EOF)
    {
        input=(char*)realloc((void*)input,i+1);
        i++;
        if(ch!=';')
        {
            input[i-1]=ch;
        }
        else
        {
            input[i-1]=';';
            input=(char*)realloc((void*)input,i+1);
            i++;
            input[i-1]='\0';
            //printf("%s#\n",input);
            str=str_adjust(input);
            //show_db(head_db,0);
            //show_table(head_db->head_table,0);
            if(str)
            {
                fprintf(fpout,"%s\n",str);
                //printf("%s\n",str);
            }
            //printf("SeedCup$  ");
            i=0;
            free(input);
            input=NULL;//or there will be realloc glibc error
        }
    }
    fclose(fpin);
    fclose(fpout);
    return 0;
}
