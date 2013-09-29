/*************************************************************************
    > File Name: strlib.h
    > Created Time: Mon 22 Oct 2012 06:24:14 PM CST
 ************************************************************************/
#define WITHOUT_NUM 1
#define WITH_NUM 2


int getnword(char *src,char *dest,int n);
int getstring(char *src,char *desc,char key);
char *strmtch(char *haystack,char *needle,int n);
//compare two string,"*" and "?" considered,n chars at most.

char *strbtw(char *src,char ch1,char ch2);
//get string between ch1 and ch2 (  (),[] and {} ),returning NULL,if not found
//src matches "\([\\w]*\)".
//
char *strquote(char *src,char *desc);
//get the string with quotation

char *strchar(char *src);
//locate the first character that is not white space.

char *strtrim(char *src);
//get rid of white spaces after last printable character
//returning the new string

char *streqt(char *src);
//return a string of equation or non-equality,
//ended by white spaces or parenthesis


int isint(char *src);//check if a string is an integer
int isfloat(char*src);//check if a string matches float
int ismexp(char *src);//check if a string is a math expression
int iscsv(char *src,int flags);
//check if a string is a comma separated value.
//flags:WITH_NUM,WITHOUT_NUM; return value number

int isscsv(char *src,int flags);
//check if a string matches "name text,age in" pattern
//that is comma seperated values(each separated by space,
//returning number of values,flags:WITH_NUM or WITHOUT_NUM


int istext(char *src);//check whether it's text

