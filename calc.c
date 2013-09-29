/*A calculator for expresion,another example for stack,beacause the big
 * numbers' division is out of my knowledge,so I just developed basic
 * algorithm by Duhao
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"
#include "strlib.h"

int valid_operation(double x,double y,char optr)
{
    switch(optr)
    {
        case '/':
            if(y==0)
                return 0;
            break;
        case '%':
            if((!(fisi(x)&&fisi(y)))||y==0)
                return 0;
            break;
    }
   return 1;
}

int fisi(double x)
{
    return x==(int)x;
}

double calc_num(char *infix,double *result)
{
    double opnd[MAX];//!!stores the operand
    char optr[MAX],ch,str[256];//!!stores the operator
    int i,j,topd,topt;//indicates the top of the stack
    i=j=0;
    topt=topd=-1;
    ch=infix[i++];
    //printf("cal.c,input is :%s\n",infix);
    while(ch!='\0')
    {
        if((ch>='0'&&ch<='9')||ch=='.')
        {
            str[j++]=ch;
        }
        else if(ch=='(')
        {
            optr[++topt]=ch;
            //strp[j]='\0';
           //opnd[++topd]=atof(str);
        }
        else if(ch==')')
        {
            if(infix[i-2]<='9'&&infix[i-2]>='0')
            {
                str[j]='\0';
                opnd[++topd]=atof(str);
                //printf("number is %f\n",opnd[topd]);
                memset(str,0,sizeof(str));
                j=0;
            }//the former char is digit
            while(topt!=-1&&optr[topt]!='(')
            {
                if(optr[topt]=='+'||optr[topt]=='-'||optr[topt]=='*'||optr[topt]=='/'||optr[topt]=='%')
                {
                    if(!(topd>1&&valid_operation(opnd[topd-1],opnd[topd],optr[topt])))
                        return 0;
                    opnd[topd-1]=binary(opnd[topd-1],optr[topt],opnd[topd]);
                    topd--;//out of stack
                    topt--;
                }
                /*else if(optr[topt]=='(')
                {
                    topt--;
                }*/
            }
            if(optr[topt]=='(')
                topt--;
        }//ch==')'
        else if(ch=='-')
        {
            if(i)
            {
                if(infix[i-2]<='9'&&infix[i-2]>='0')
                {
                    str[j]='\0';
                    opnd[++topd]=atof(str);
                    //printf("string is %s\n",str);
                    //printf("number is %f\n",opnd[topd]);
                    memset(str,0,sizeof(str));
                    j=0;
                }
                if((infix[i-2]<'0'||infix[i-2]>'9')&&infix[i-2]!=')')//the former char is not digit
                    str[j++]=ch;//'-' is to be a minus
                else//'-' is an operator
                {
                    while(topt!=-1&&optr[topt]!='(')
                    {
                        if(topd<1)
                            break;
                        if(!(topd>1&&valid_operation(opnd[topd-1],opnd[topd],optr[topt])))
                            return 0;
                        opnd[topd-1]=binary(opnd[topd-1],optr[topt],opnd[topd]);
                        topd--;
                        topt--;
                    }
                    optr[++topt]=ch;
                }//else operator
            }//the first character
            else
                str[j++]=ch;
        }//else if ch=='-'
        else if(ch=='+'||ch=='*'||ch=='/'||ch=='%')
        {
            if(infix[i-2]<='9'&&infix[i-2]>='0')
            {
                str[j]='\0';
                opnd[++topd]=atof(str);
                memset(str,0,sizeof(str));
                j=0;
            }//for sure that the former char is not ')'
            while(topt!=-1&&optr[topt]!='(')
            {
                switch(ch)
                {
                    case '+':
                        if(topd<1)
                            return 0;
                        opnd[topd-1]=binary(opnd[topd-1],optr[topt],opnd[topd]);
                        topd--;
                        topt--;
                        break;
                    case '*':
                    case '/':
                    case '%':
                        if(optr[topt]=='*'||optr[topt]=='/'||optr[topt]=='%')
                        {
                            if(!(topd>1&&valid_operation(opnd[topd-1],opnd[topd],optr[topt])))
                                return 0;
                            opnd[topd-1]=binary(opnd[topd-1],optr[topt],opnd[topd]);
                            topd--;
                            topt--;
                        }
                        else
                            goto lable;//critical!the jump out of the loop!!!
                        break;
                }
            }//while(topt!=-1&&optr[topt]!='(')
lable:
            optr[++topt]=ch;//into the stack
        }//else if ch=='*'||ch=='/'||ch=='+'
        ch=infix[i++];
    }//while(ch!='\0')
    if(infix[i-2]>='0'&&infix[i-2]<='9')
    {//convert the last string to number
        str[j]='\0';
        opnd[++topd]=atof(str);
        //printf("the last string is %s\n",str);
        //printf("number :%f\n",opnd[topd]);
        memset(str,0,sizeof(str));
        j=0;
    }
    while(topt!=-1)
    {//deal with operators still in stack
        if(topd)
        {//must have at least two operands
            if(!valid_operation(opnd[topd-1],opnd[topd],optr[topt]))
                return 0;
            opnd[topd-1]=binary(opnd[topd-1],optr[topt],opnd[topd]);
            topd--;
        }
        else
            return 0;
        topt--;
    }
    //printf("calc.c,144:%f\n",opnd[topd]);
    *result=opnd[topd];
    return 1;
}

double binary(double s,char ch,double t)
{
    switch(ch)
    {
        case '+':
            return s+t;
        case '-':
            //printf("subduction result is %f\n",s-t);
            return s-t;
        case '*':
            return s*t;
        case '/':
            return s/t;
        case '%':
            return (int)s%(int)t;
    }
    return 0;
}

int comp_num(double x,double y,char *optr)
{
    char *str=(char*)malloc(strlen(optr)+1);
    strcpy(str,optr);
    str=strchar(str);
    str=strtrim(str);

    //printf("174:%f %f %s\n",x,y,optr);
    if(!strcmp(str,"==")||!strcmp(str,"="))
        return !(x-y);
    else if(!strcmp(str,">="))
        return x>=y;
    else if(!strcmp(str,"<="))
        return x<=y;
    else if(!strcmp(str,">"))
    {
        //printf("182;%f %f %s result:%d\n",x,y,optr,x>y);
        return x>y;
    }
    else if(!strcmp(str,"<"))
        return x<y;
    else
        return x!=y;//for !=,~=
}
