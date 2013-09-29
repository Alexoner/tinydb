/*************************************************************************
    > File Name: strlib.c
    > Created Time: Mon 22 Oct 2012 06:27:09 PM CST
 ************************************************************************/
#include "strlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "str.h"

int getnword(char *src,char *dest,int n)
{
    int i,j,k;
    for(i=0;isblank(src[i])&&src[i]!='\0';i++);
    for(j=0,k=0;j<n&&src[i]!='\0';i++)
    {
        if(isspace(src[i]))
        {
            if(!isspace(src[i-1]))
                j++;
            continue;
        }
        dest[k++]=src[i];
    }
    dest[k]='\0';
    return i;
}


int getstrbfr(char *src,char *desc,char key)
{//get string before key
    int i,j;
    static int over=0;
    if(over)
    {
        over=0;
        //this is a CRITICAL STATEMENT,or there won't be any more
        //operations for this function
        return 0;
    }
    for(i=0,j=0;src[i]!=key&&src[i]!='\0';i++)
    {
        if(isblank(src[i])&&isblank(src[i-1]))
            continue;
        desc[j++]=src[i];
    }
    desc[j]='\0';
    if(src[i]=='\0')
        over=1;
    else if(src[i]==key)
        i++;
    return i;
}


char *strbtw(char *src,char ch1,char ch2)
{
    int i,k,n1=0,n2=0;
    char *desc=malloc(strlen(src)+1);
    for(k=0,i=1;src[i]!='\0';i++)
    {
        if(src[i]==ch1)
            n1++;
        else if(src[i]==ch2)
        {//get the ')'
            n2++;
            if(n2>n1)
            {//time to stop reading now
                //desc[k]='\0';
                break;
            }
            /*else if(n2==n1)
            {
                desc[k++]=ch2;
                break;
            }*/
        }
        desc[k++]=src[i];
    }
    desc[k]='\0';
    return desc;
}

char *strquote(char *src,char *desc)
{
    int i,j;
    char ch=src[0];//single quotation or double quotation
    desc[0]=src[0];
    for(i=1,j=1;src[i]!='\0'&&src[i]!=ch;i++,j++)
    {
        desc[j]=src[i];
    }
    if(src[i]==ch)
        desc[j++]=ch;
    desc[j]='\0';
    return desc;
}


char *strchar(char *src)
{
    int i;
    for(i=0;src[i]!='\0'&&isspace(src[i]);i++);
    return src+i;
}

char *strtrim(char *src)
{
    int i=strlen(src);
    for(;i&&isspace(src[i-1]);i--);
    for(i=0;src[i]!='\0'&&isspace(src[i]);i++);
    return src+i;
}

char *streqt(char *src)
{
    //the string begins with non-white-space character
    //a value must be a constant (without white space)
    //or a expression surrouded by pairs of parenthesis

    int i=0,j,optr=0,parenthesis=0,quotation=0;//i for =,right value.
    char *desc=(char*)malloc(strlen(src)+1);

    for(i=0,j=0;i<strlen(src);i++,j++)
    {
        if(isspace(src[i]))
        {
            if(parenthesis==0&&!(quotation%2))
                break;
        }
        else if(src[i]=='='||src[i]=='>'||src[i]=='<')
            break;
        else if(src[i]=='(')
            parenthesis++;
        else if(src[i]==')')
        {
            parenthesis--;
            if(parenthesis==0)
            {
                desc[j++]=src[i++];
                //before break,j++ to move on to next char
                break;
            }
        }
        else if(src[i]=='\''||src[i]=='\"')
        {
            quotation++;
            if(!(quotation%2))
            {
                desc[j++]=src[i++];
                break;
            }
        }
        desc[j]=src[i];
    }//get left value




    if(parenthesis||(quotation%2))
    {
        free(desc);
        return NULL;
    }

    for(;i<strlen(src);i++,j++)
    {//get rid of space
        if(isspace(src[i]))
            desc[j]=src[i];
        else
            break;
    }

    if(i==strlen(src))
    {
        free(desc);
        return NULL;
    }

    for(;i<strlen(src);i++,j++)
    {//get the operator
        if(src[i]!='='&&src[i]!='<'&&src[i]!='>')
            break;
        if(optr)//already read a '='
            if(optr>1)
            {
                free(desc);
                return NULL;
            }
        optr++;
        desc[j]=src[i];
    }//got the operator

    for(;i<strlen(src);i++,j++)
    {//get rid of space
        if(isspace(src[i]))
            desc[j]=src[i];
        else
            break;
    }

    if(i==strlen(src))
    {
        free(desc);
        return NULL;
    }

    parenthesis=quotation=0;
    for(;i<strlen(src);i++,j++)
    {//get the right value
        if(isspace(src[i]))
        {//get rid of space
            if(parenthesis==0&&!(quotation%2))
                break;
        }
        else if(src[i]=='(')
            parenthesis++;
        else if(src[i]==')')
        {
            parenthesis--;
            if(parenthesis==0)
            {
                desc[j++]=src[i++];
                //before break,j++ to move on to next char
                break;
            }
        }
        else if(src[i]=='\''||src[i]=='\"')
        {
            quotation++;
            if(!(quotation%2))
            {
                desc[j++]=src[i++];
                break;
            }
        }
        desc[j]=src[i];
    }//got the right value
    if(parenthesis||quotation%2)
    {
        free(desc);
        return NULL;
    }
    desc[j]='\0';
    return desc;
}

int isint(char *src)
{
    int i=0;
    for(i=0;src[i]!='\0';i++)
    {
        if(!isdigit(src[i]))
            return 0;
    }
    return 1;
}

int isfloat(char *src)
{
    int i=0,dot=0;
    for(i=0;src[i]!='\0';i++)
    {
        if(!isdigit(src[i]))
        {
            if(src[i]!='.')
                return 0;
            else
            {
                if(dot)
                    return 0;
            }
        }
    }
    return 1;
}

int ismexp(char *src)
{
    int i;
    for(i=0;src[i]!='\0';i++)
    {
        if(!((src[i]>='0'&&src[i]<='9')||src[i]=='+'||src[i]=='-'||
                    src[i]=='*'||src[i]=='/'||src[i]=='('||src[i]==')'||
                    src[i]=='.'))
            return 0;
    }
    return 1;
}

int iscsv(char *src,int flags)
{
    char *str1,*str2,*buf,*str3,*format;
    int i,num=0;

    str1=(char*)malloc(strlen(src)+1);
    strcpy(str1,src);
    strtrim(str1);
    str2=strchar(str1);
    if(str2[0]=='(')
    {
        str2=strchar(str2+1);
        str2[strlen(str2)-1]='\0';
    }//get rid of parenthesis

    switch(flags)
    {
        case WITHOUT_NUM:
            format=(char*)malloc(strlen("%[a-zA-Z_\'\"]")+1);
            strcpy(format,"%[a-zA-Z_\'\"]");
            break;
        case WITH_NUM:
            format=(char*)malloc(strlen("%[a-zA-Z_\'\".0-9]")+1);
            strcpy(format,"%[a-zA-Z_\'\".0-9]");
            break;
        default:
            format=(char*)malloc(strlen("%[a-zA-Z_\'\"0-9]")+1);
            strcpy(format,"%[a-zA-Z_\'\"0-9]");
            break;
    }
    buf=(char*)malloc(strlen(str2)+1);
    while(1)
    {
        if(str2[0]=='\'')
        {
            strquote(str2,buf);
            //printf("300:%s\n",buf);
            if(!istext(buf))
            {
                //printf("303:\n");
                return 0;
            }
        }
        else if(str2[0]=='\"')
        {
            strquote(str2,buf);
            if(!istext(buf))
                return 0;
        }
        else
        {
            i=sscanf(str2,format,buf);
            if(!i)
            {
                if(str2[0]==',')
                {
                    num++;
                    str2=strchar(str2+1);
                    continue;
                }
                else if(str2[0]=='\0')
                {
                    break;
                }
                else
                    return 0;
            }
        }
        str2+=strlen(buf);
        str2=strchar(str2);//the first non-white space char
        if(str2[0]!=','&&str2[0]!='\0')
            return 0;
        num++;
        if(str2[0]==',')
        {
            str2=strchar(str2+1);
            if(str2[0]=='\0')
            {
                num++;
                break;
            }
        }
        else if(str2[0]=='\0')
        {
            //num++;
            break;
        }
    }
    return num;
}


int isscsv(char *src,int flags)
{//space
    char *str1,*str2,*buf,*str3,*format;
    int i,num=0;

    str1=(char*)malloc(strlen(src)+1);
    strcpy(str1,src);
    str2=strtrim(str1);
    if(str2[0]=='(')
    {
        str2=strchar(str2+1);
        str2[strlen(str2)-1]='\0';
    }//get rid of parenthesis

    switch(flags)
    {
        case WITHOUT_NUM:
            format=(char*)malloc(strlen("%[a-zA-Z_\'\"]")+1);
            strcpy(format,"%[a-zA-Z_\'\"]");
            break;
        case WITH_NUM:
        default:
            format=(char*)malloc(strlen("%[a-zA-Z_\'\"0-9]")+1);
            strcpy(format,"%[a-zA-Z_\'\"0-9]");
            break;
    }
    buf=(char*)malloc(strlen(str2)+1);
    while(1)
    {
        if(str2[0]=='\'')
        {
            strquote(str2,buf);
            if(!istext(buf))
                return 0;
        }
        else if(str2[0]=='\"')
        {
            strquote(str2,buf);
            if(!istext(buf))
                return 0;
        }
        else if(!(i=sscanf(str2,format,buf)))
        {
            //printf("276:%s\n",str2);
            return 0;
        }
        str2+=strlen(buf);
        str2=strchar(str2);//skip white spaces
        //printf("279:%s\n",str2);
        i=sscanf(str2,format,buf);
        if(!i)
        {//second str2ing not matched,only two cases allowed
            if(str2[0]=='\0')
            {
                num++;
                break;//the end
            }
            else if(str2[0]==',')
            {//lacking second word,allowed:"name,age int"
                str2=strchar(str2+1);
                num++;
                continue;
            }
            else
                return 0;
        }

        str2+=strlen(buf);
        str2=strchar(str2);//first non-space character

        if(str2[0]!=','&&str2[0]!='\0')
            return 0;//the extra str2ing
        else if(str2[0]==',')
            str2=strchar(str2+1);
        if(str2[0]=='\0')
        {
            num++;
            break;
        }
        num++;
    }//check syntax errors for column
    free(buf);
    free(str1);
    return num;
}


int istext(char *src)
{
    int i,length;
    length=strlen(src);
    for(i=1;i<length-1;i++)
    {
        if((!isalpha(src[i]))&&(!isdigit(src[i])))
            if(src[i]!='_'&&src[i]!=' ')
                return 0;
    }
    return 1;
}
