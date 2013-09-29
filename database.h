/*************************************************************************
    > File Name: database.h
    > Created Time: Sat 20 Oct 2012 12:18:05 AM CST
 ************************************************************************/
#ifndef DATABASE_H
#define DATABASE_H

#include "linkedlist.h"
#include "strlib.h"
#include "str.h"

#define NOORDER 0
#define DESC 1
#define INCR 2

#define ADD 1
#define DROP 2
#define ALTER_COLUMN 3

#define OPTR 1
#define OPND 2

#define OR 1
#define AND 2

#define NONE 1
#define INT 2
#define FLOAT 3
#define TEXT 4

#define ERROR -1
#define EMPTY 1

typedef struct value
{
    char *data;
    struct value *next;
}Value;

typedef struct column
{
    char *db_name;
    char *table_name;
    char *column_name;
    int datatype;
    struct value *head_value;
    struct column *next;
}Column;

typedef struct table
{
    char *db_name;
    char *table_name;
    struct column *head_column;
    struct table *next;
}Table;

typedef struct db
{
    char *name;
    struct table *head_table;
    struct db *next;
}Db;//database

struct expression
{//a struct of operator and operator,for solving expression
    int type;//operator or operand
    nums *opnd;//pointer to a set(nums)
    int optr;
    struct expression *next;
};

typedef struct int_char
{
    int type;
    char *str;
}Int_char;

extern struct db *head_db;
extern struct db *current_db;
extern int error;
//extern char keyword[];//keywords
//stage 1:

//BEGIN string operations
char *str_adjust(char *str);//return the value

// Adjust user input equation string before solving it.
//END string operations

//BEGIN database operations
char *create_db(struct db **pd, char* dname);//append a database
char *create_table(struct db *pd, char *tname, char *column);//append a table
char *create_column(struct table *pt, char *column);//insert a column
char *alter_table(struct table *pt,int option, char *argument);
//modify columns of the given table;option:add,drop,alter

char * truncate_table(struct table *pt); char * use_database(struct db *hd,char *name);//hd,ht are head pointers
char * drop_database(struct db **hd,char *name);//hd:db's head pointer
char * drop_table(struct db *hd,char *db_name,char *table_name);//ht: table's head pointer
char * rename_table(struct table *ht,char *oldname,char *newname);
char * rename_database(struct db *hd,char *oldname,char *newname);
//END database operations

//stage 2:

//BEGIN data operations
char* Select(struct table *pt,char *column,char *term,char *order);
//pt:table pointer;column:column names;term:argments after "where"
//order:arguments after "order"

char * update(struct table *ht,char *table_name,char *change,char *term);
char * Delete(struct table *ht,char *table_name,char *term);
//term: arguments after where

char * insert_into(struct table *ht,char *table_name,
        char *column_name,char *value);
//END data operations


//stage3:

//BEGIN display
char *show_db(struct db *hd,int order);
char *show_table(struct table *ht,int order);
char *show_column(struct column *hc,int order);//Tuesday

//BEGIN auxiliary functions
int display_db_node(void *pd);
int display_table_node(void *pt);
int display_column_node(void *pc);

int db_equal(void *p,void *q);
int table_equal(void *p,void *q);
int column_equal(void *p,void *q);
int get_column_string(char *src,char *desc);
//get a "column type",return number of chars read
//excluding the terminating null byte '\0'


nums *solve_complex(struct table *pt,char *expression);
//solve complex logical algorithm

nums *solve_single(struct table *pt,char *expression);
//return a head pointer of a linkedlist,which contains  numbers of
//rows matches specifications declared in expression.
//call compute function to return value.

nums *solve_expression(struct expression *he,int n);
//solve the expression presented by linked list

nums *compute_num(struct column *pc,char *optr,char *right_value);
//return a set (linkedlist of matched rows),pc is Column pointer
//optr is "=",">","<",right_value is string after optr.


nums *compute_str(struct column *pc,char *optr,char *right_value);


int getnword(char*src,char *dest,int n);
//get n words from src to dest,and return the number of chars read,excluding '\0'

//END auxiliary functions

#endif
