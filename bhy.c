#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include "database.h"
#include <string.h>
#include "str.h"
#include <strings.h>


strs *strs_head=NULL;
strs *strs_crt=NULL;
nums *nums_head=NULL;
nums *nums_crt=NULL;

char keyword[][10]={
	"\0",					//0
	"create",				//1
	"alter",				//2
	"truncate",				//3
	"use",					//4
	"drop",					//5
	"rename",				//6
	"select",				//7
	"update",				//8
	"delete",				//9
	"insert",				//10
	"show",					//11
	"database",				//12
	"table",				//13
	"add",					//14
	"drop",					//15
	"databases",			//16
	"column",				//17
	"order",				//18
	"by",					//19
	"desc",					//20
	"incr",					//21
	"from",					//22
	"where",				//23
	"into",					//24
	"set",					//25
	"values",				//26
	"between",				//27
	"like",					//28
	"[",					//29
	"]",					//30
	",",					//31
	"(",					//32
	")",					//33
	"*",					//34
	"/",					//35
	"%",					//36
	"+",					//37
	"-",						//38
	"=",					//39
	"==",					//40
	"~",					//41
	"~=",					//42
	">=",					//43
	"<=",					//44
	">",					//45
	"<",					//46
	"and",					//47
	"or",					//48
	"int",					//49
	"float",				//50
	"text"					//51

};

void str_std(char *str)
{
	int i=0,j=strlen(str);

	while(str[i]!=';'&&str[i]!='\0')
	{
		if (str[i]=='\n')str[i]=' ';
		i++;
	}

	i=0;
	while(str[i]!=';'&&str[i]!='\0')
	{
		if (str[i]==' ')
		{
			while(str[i+1]==' ')
			{
				char *ci=(char *)malloc(j);
				strcpy(ci,&(str[i+2]));
				strcpy(&(str[i+1]),ci);
			}
		}
		i++;
	}
	str[i+1]='\0';
	if (str[0]==' ')
	{
		char *ci=(char *)malloc(j);
		strcpy(ci,&(str[1]));
		strcpy(&(str[0]),ci);
	}
	i=0;
	while(str[i]!=';'&&str[i]!='\0'&&i<=j-6)
	{
		if ((i>0)&&!(strncasecmp(&(str[i]),"values",6))&&(str[i-1]==')'))
		{
		    char *str1;
		    str=(char *)realloc(str,strlen(str)+2);
		    str1=(char *)malloc(strlen(str)+2);
		    strcpy(str1,&(str[i]));
		    str1[strlen(str1)]='\0';
		    strcpy(&(str[i+1]),str1);
		    str[strlen(str)]='\0';
		    str[i]=' ';
		    j=strlen(str);
		}
		i++;
	}
}

strs *str_break(char *str)
{
	strs *h=NULL,*c=NULL;
	int i=0,j=0,f=0;
	while(str[i]!=';'&&str[i]!='\0')
	{
		if (str[i]==' ')
		{
			strs *pi=(strs *)malloc(sizeof(strs));
			pi->last=c;
			if(c!=NULL)c->next=pi;
			c=pi;
			pi->next=NULL;
			pi->data=(char *)malloc((i-j+1)*sizeof(char));
			strncpy(pi->data,&(str[j]),i-j);
			pi->data[i-j]='\0';
			if(match_key(pi->data)!=0)strcpy(pi->data,str_tolower(pi->data));						//
			if(f==0)
			{
				h=pi;
				f=1;
			}
			j=i+1;
		}
		i++;
	}
	if (j!=i)
	{
		strs *pi=(strs *)malloc(sizeof(strs));
			pi->last=c;
			if(c!=NULL)c->next=pi;
			c=pi;
			pi->next=NULL;
			pi->data=(char *)malloc((i-j+1)*sizeof(char));
			strncpy(pi->data,&(str[j]),i-j);
			pi->data[i-j]='\0';
			if(match_key(pi->data)!=0)strcpy(pi->data,str_tolower(pi->data));
			if(f==0)
			{
				h=pi;
				f=1;
			}
	}
	return h;
}

strs *strs_break(char *str)
{
	strs *h=NULL,*c=NULL;
	int i=0,j=0,f=0;
	while(str[i]!='\0')
	{
		if (str[i]==',')
		{
			strs *pi=(strs *)malloc(sizeof(strs));
			pi->last=c;
			if(c!=NULL)c->next=pi;
			c=pi;
			pi->next=NULL;
			pi->data=(char *)malloc((i-j+1)*sizeof(char));
			strncpy(pi->data,&(str[j]),i-j);
			pi->data[i-j]='\0';
			if(match_key(pi->data)!=0)strcpy(pi->data,str_tolower(pi->data));						//
			if(f==0)
			{
				h=pi;
				f=1;
			}
			j=i+1;
		}
		i++;
	}
	if (j!=i)
	{
		strs *pi=(strs *)malloc(sizeof(strs));
			pi->last=c;
			if(c!=NULL)c->next=pi;
			c=pi;
			pi->next=NULL;
			pi->data=(char *)malloc((i-j+1)*sizeof(char));
			strncpy(pi->data,&(str[j]),i-j);
			pi->data[i-j]='\0';
			if(match_key(pi->data)!=0)strcpy(pi->data,str_tolower(pi->data));
			if(f==0)
			{
				h=pi;
				f=1;
			}
	}
	return h;
}



char *str_merge(strs *hstr,int n)
{
	char *pc=NULL;
	if (hstr!=NULL)
	{
		int l=strlen(hstr->data);
		pc=(char *)malloc(l+1);
		strcpy(pc,hstr->data);
		pc[l]='\0';
		strs *c=hstr->next;
		int i;
		for(i=1;i<n;i++)
		{
			l=strlen(pc);
			pc=(char *)realloc(pc,l+strlen(c->data)+2);
			if((pc[l-1]==',') || ((c->data[0])==','))
			{
				strcat(pc,c->data);
				pc[l+strlen(c->data)]='\0';

			}
			else
			{
				strcat(pc," ");
				strcat(pc,c->data);
				pc[l+strlen(c->data)+1]='\0';
			}

			c=c->next;
		}
	}
	return pc;
}

char *str_tolower(char *str)
{
	int i, l=strlen(str);
	char *s=(char*)malloc(strlen(str)+1);
	strcpy(s,str);
	for(i=0;i<l;i++)s[i]=tolower(s[i]);
	return s;
}

int match_key(char *str)					//51
{
	int i;
	for(i=51;i>0;i--)
	{
		if(strcmp(str_tolower(str),keyword[i])==0)return i;
	}
	return i;
}

int str_len()
{
	int i=0;
	strs *pi=strs_head;
	while (pi!=NULL)
	{
		pi=pi->next;
		i++;
	}
	return i;
}

int strs_len(strs *hs)
{
	int i=0;
	strs *pi=hs;
	while (pi!=NULL)
	{
		pi=pi->next;
		i++;
	}
	return i;
}

strs *str_adr(int n)
{
	strs *pi;
	int j;
	pi=strs_head;
	for(j=1;(j<n)&&(pi!=NULL);j++)pi=pi->next;
	return pi;
}

strs *strs_adr(strs *hs,int n)
{
	strs *pi;
	int j;
	pi=hs;
	for(j=1;(j<n)&&(pi!=NULL);j++)pi=pi->next;
	return pi;
}

char *str_bracket(char *str)
{
	char *str1=(char *)malloc(strlen(str)+1);
	strcpy(str1,str+1);
	int i=0,j=1;
	while((i<strlen(str1))&&(j>0))
	{

		if(str1[i]=='(')j++;
		if(str1[i]==')')j--;
		i++;
	}
	if(j==0)
	{
		i--;
		str1[i]='\0';
		return str1;
	}
	else return "\0";

}

int num_len(nums *nh)
{
	nums *n1;
	n1=nh;
	int i=0;
	while (n1!=NULL)
	{
		n1=n1->next;
		i++;
	}
	return i;
}

int match_num(nums *nh,nums *ni)
{
    if((nh==NULL)||(ni==NULL))
    return 0;
    else
    {
        int i=1;
        nums *n1;
        n1=nh;
        if(n1->data==ni->data)return i;
        while(n1->next!=NULL)
        {
            i++;
            n1=n1->next;
            if(n1->data==ni->data)return i;
        }
        return 0;
    }
}

int match_str(strs *sh,strs *si)
{
	int i=1;
	strs *s1;
	s1=sh;
	if(strcmp(s1->data,si->data)==0)return i;
	while(s1->next!=NULL)
	{
		i++;
		s1=s1->next;
		if(strcmp(s1->data,si->data)==0)return i;
	}
	return 0;
}


nums *num_and(nums *n1,nums *n2)
{
	nums *n3,*ni;
	int i=0,l1;
	ni=n1;
	n3=NULL;
	l1=num_len(n1);
	for(i=0;i<l1;i++)
	{
		if (match_num(n2,ni)!=0)
		{
			nums *np;
			np=(nums*)malloc(sizeof(nums));
			np->data=ni->data;
			np->next=n3;
			n3=np;

		}
		ni=ni->next;
	}
	return n3;
}

nums *num_or(nums *n1,nums *n2)
{
	nums *n3,*ni;
	int i=0,l1,l2;
	ni=n1;
	n3=NULL;
	l1=num_len(n1);
	l2=num_len(n2);
	for(i=0;i<l1;i++)
	{
		if (match_num(n3,ni)==0)
		{
			nums *np;
			np=(nums*)malloc(sizeof(nums));
			np->data=ni->data;
			np->next=n3;
			n3=np;

		}
		ni=ni->next;
	}
	ni=n2;
	for(i=0;i<l2;i++)
	{
		if (match_num(n3,ni)==0)
		{
			nums *np;
			np=(nums*)malloc(sizeof(nums));
			np->data=ni->data;
			np->next=n3;
			n3=np;

		}
		ni=ni->next;
	}
	return n3;
}


int wildcard(const char *text, const char *wi, int flag)
{
    if( NULL == text || NULL == wi ){
        return -1;
    }
    if( (strcmp(text,"\0")==0) && (strcmp(wi,"\0")==0) ){
        return 0;
    }
    if( 1 == flag ){
        int ret = wildcard(text, wi, 0);
        if((strcmp(text,"\0")==0)&&(ret==1))return 1;
        return ( ret == 0 ) ? 0 : wildcard(text+1, wi, 1);
    }
    if( ('\0' == *text && '\0' != *wi && '*' != *wi) ||
        ('\0' != *text && '\0' == *wi) ){
        return 1;
    }
    switch(*wi){
    case '*':
        return wildcard(text, wi+1, 1);
    case '?':
        return wildcard(text+1, wi+1, 0);
    default:
        if( *text == *wi ){
            return wildcard(text+1, wi+1, 0);
        }else{
            return 1;
        }
    }
}

void val_d(struct column *hc ,int nc)
{
	struct column *ic;
	ic=hc;
	while (ic!=NULL)
	{
		if(nc==1)
		{
			Value *vi;
			vi=ic->head_value;
			ic->head_value=vi->next;
			free(vi);
		}
		else
		{
			int k;
			Value *vi,*vd;
			vi=ic->head_value;
			for(k=1;k<nc-1;k++)vi=vi->next;
			vd=vi->next;
			vi->next=vd->next;
			free(vd);
		}
		ic=ic->next;
	}
}

void num_sort(nums *nh,int order)
{
	int i;
	i=num_len(nh);
	if(i>1)
	{
		int j,k;
		for(j=1;j<i;j++)
		{
			for(k=j+1;k<i+1;k++)
			{
				if(order==0)
				{
					if((num_adr(nh,j)->data)>(num_adr(nh,k)->data))
					{
						int ii;
						ii=num_adr(nh,j)->data;
						num_adr(nh,j)->data=num_adr(nh,k)->data;
						num_adr(nh,k)->data=ii;
					}
				}
				else
				{
					if((num_adr(nh,j)->data)<(num_adr(nh,k)->data))
					{
						int ii;
						ii=num_adr(nh,j)->data;
						num_adr(nh,j)->data=num_adr(nh,k)->data;
						num_adr(nh,k)->data=ii;
					}
				}

			}
		}
	}
}

nums *num_adr(nums *nh,int n)
{
	nums *pi;
	int j;
	pi=nh;
	for(j=1;(j<n)&&(pi!=NULL);j++)pi=pi->next;
	return pi;
}

Value *value_adr(Value *vh,int n)
{
	Value *pi;
	int j;
	pi=vh;
	for(j=1;(j<n)&&(pi!=NULL);j++)pi=pi->next;
	return pi;
}


int str_pos(char *str,char c,int t)
{
	int i;
	i=0;
	if(t>0)
	{

		while((t!=0)&&(str[i]!='\0'))
		{
			if(str[i]==c)t--;
			i++;
		}
		if(t!=0)return -1;
	}
	return i-1;
}

int is_str(char *src)
{
    int i=0;
    for(i=0;src[i]!='\0';i++)
    {
        if(!isalpha(src[i]))
        {
            if(src[i]!='_')
                return 0;
        }
    }
    return 1;
}

int is_text(char *src)
{
    int i=0;
    for(i=0;src[i]!='\0';i++)
    {
        if((!isalpha(src[i]))&&(!isdigit(src[i])))
        {
            if((src[i]!='_')&&(src[i]!=' '))
                return 0;
        }
    }
    return 1;
}

char * chfloat(char *str)
{
    int di;
    di=str_pos(str,'.',1);
    if(di!=-1)
    {
        str=(char *)realloc(str,strlen(str)+3);
        strcat(str,"00");
        str[di+3]='\0';

    }
    else
    {
        str=(char *)realloc(str,strlen(str)+4);
        strcat(str,".00");
        str[strlen(str)]='\0';
    }
    return str;
}

void show_strs()
{
	strs *c=strs_head;
	while(c!=NULL)
	{
		printf("%s\n",c->data);
		c=c->next;
	}
}

void str_free(strs *h)
{
    strs *i;
    while (h!=NULL)
    {
        i=h;
        h=h->next;
        free(i);
    }
}


char *str_adjust(char *str)
{
	str_std(str);
	strs_head=str_break(str);
	if(str_len()>=2){
		strs_crt=strs_head->next;
	switch (match_key(strs_head->data))
	{
		case 1:
			if(match_key(strs_head->next->data)==12)
			{
				return create_db(&head_db,str_merge(strs_head->next->next,str_len()-2));
			}
			else
			{
				if((match_key(strs_head->next->data)==13)&&(current_db!=NULL))
				{
					return create_table(current_db,strs_head->next->next->data,str_merge(strs_head->next->next->next,str_len()-3));
				}
				else return "error";
			}

			break;
		case 2:
			if((match_key(strs_head->next->data)==13) && (current_db!=NULL))
			{
				Table *pt,*qt;
				pt=(struct table*)malloc(sizeof(struct table));
				pt->table_name=strs_head->next->next->data;
				qt=ListLocateNode(current_db->head_table,pt,table_equal,offsetof(Table,next));
				if(qt!=NULL)
				{
					switch (match_key(strs_head->next->next->next->data))
					{
					case 14:
						return alter_table(qt,ADD,str_merge(strs_head->next->next->next->next,str_len()-4));
						break;
					case 15:
						if(match_key(strs_head->next->next->next->next->data)==17)
							return alter_table(qt,DROP,str_merge(strs_head->next->next->next->next->next,str_len()-5));
						else return "error";
						break;
					case 2:
						if(match_key(strs_head->next->next->next->next->data)==17)
							return alter_table(qt,ALTER_COLUMN,str_merge(strs_head->next->next->next->next->next,str_len()-5));
						else return "error";
						break;
					}

				}
				else return "error";
			}
			else return "error";
			break;
		case 3:
			if((match_key(strs_head->next->data)==13)&&(current_db!=NULL))
			{
				Table *pt,*qt;
				pt=(struct table*)malloc(sizeof(struct table));
				pt->table_name=strs_head->next->next->data;
				qt=ListLocateNode(current_db->head_table,pt,table_equal,offsetof(Table,next));
				if(qt!=NULL)
				{
					return truncate_table(qt);
				}
				else return "error";
			}
			else return "error";
			break;
		case 4:
			if(str_len()==2)
			return use_database(head_db,strs_head->next->data);
			else return "error";
			break;
		case 15:case 5:
			if(strs_head->next->next==NULL)
			{
				return drop_database(&head_db,strs_head->next->data);
			}
			else
			{
                    if(str_len()==3)
                    {
                        return drop_table(head_db,strs_head->next->data,strs_head->next->next->data);
                    }
                    else return "error";

            }
            break;
        case 6:
                if(str_len()==4)
                {
                    if(match_key(strs_head->next->data)==12)
                    {
                        return rename_database(head_db,strs_head->next->next->data,strs_head->next->next->next->data);
                    }
                    else
                    {
                        if((match_key(strs_head->next->data)==13)&&(current_db!=NULL))
                        {
                            return rename_table(current_db->head_table,strs_head->next->next->data,strs_head->next->next->next->data);
                        }
                        else return "error";
                    }

                }else return "error";
                break;
        case 7:
				if(((strs_crt->data)[0]=='*')&&(strlen(strs_crt->data)>1))
				{
				    strs *pi=(strs *)malloc(sizeof(strs));
                    pi->last=strs_head;
                    pi->next=strs_crt;
                    strs_head->next=pi;
                    strs_crt->last=pi;
                    strcpy(strs_crt->data,&(strs_crt->data[1]));
                    strs_crt=pi;
                    pi->data=(char *)malloc(2*sizeof(char));
                    strcpy(pi->data,"*");
                    pi->data[1]='\0';
				}
				if(match_key(strs_crt->data)==0 || match_key(strs_crt->data)==34)
				{
				    if(current_db!=NULL){
					int fi,wi,oi;
					strs *si;
					Table *pt;
					si=(strs *)malloc(sizeof(strs));
					si->data=(char *)malloc(6*sizeof(char));
					strcpy(si->data,"from");
					fi=match_str(strs_head,si);
					char* clm=(char*)malloc(1+strlen(str_merge(strs_head->next,fi-2)));
					strcpy(clm,str_merge(strs_head->next,fi-2));
					strcpy(si->data,"where");
					wi=match_str(strs_head,si);
					strcpy(si->data,"order");
					oi=match_str(strs_head,si);
					strs_crt=str_adr(fi+1);
					pt=(struct table*)malloc(sizeof(struct table));
					pt->table_name=strs_crt->data;
					Table *qt=ListLocateNode(current_db->head_table,pt,table_equal,offsetof(Table,next));
					if(qt!=NULL)
					{
						if(strs_crt->next==NULL)return Select(qt,clm,NULL,NULL);
						else
						{
							if((wi!=0)&&(oi==0))
							{
								if((wi==fi+2)&&(str_len()>wi))
								{
									return Select(qt,clm,str_merge(str_adr(wi+1),str_len()-wi),NULL);
								}
								else return "error";
							}
							else
							{
								if((oi!=0) && (wi==0))
								{
									if((oi==fi+2)&&(str_len()>oi+1)&&(match_key((str_adr(oi+1))->data)==19))
									{
										return Select(qt,clm,NULL,str_merge(str_adr(oi+2),str_len()-oi-1));
									}
									else return "error";
								}
								else
								{
									if((oi!=0) && (wi!=0) && (oi>wi)&&(wi==fi+2)&&(str_len()>oi+1)&&(match_key((str_adr(oi+1))->data)==19))
									{
										return Select(qt,clm,str_merge(str_adr(wi+1),oi-wi-1),str_merge(str_adr(oi+2),str_len()-oi-1));
									}
									else return "error";
								}
							}
						}
					}
					else return "error";
				    }
				    else return "error";
				}
				else return "error";

                break;
            case 8:
				if((current_db!=NULL)&&(match_key(strs_crt->next->data)==25))
				{
					int wi;
					strs *si;
					si=(strs *)malloc(sizeof(strs));
					si->data=(char *)malloc(6*sizeof(char));
					strcpy(si->data,"where");
					wi=match_str(strs_head,si);
					if((wi>4)&&(str_len()>(wi)))
					{
						strs_crt=str_adr(4);
						char* change=(char*)malloc(1+strlen(str_merge(strs_crt,wi-4)));
						strcpy(change,str_merge(strs_crt,wi-4));
						strs_crt=str_adr(wi+1);
						return update(current_db->head_table,strs_head->next->data,change,str_merge(strs_crt,str_len()-wi));
					}
					else return "error";

				}
				else return "error";
                break;
            case 9:
				if((match_key(strs_crt->data)==22)&&(current_db!=NULL)&&(str_len()>=5))
				{
					strs_crt=str_adr(4);
					if(match_key(strs_crt->data)==23)
					{
						return Delete(current_db->head_table,str_adr(3)->data,str_merge(str_adr(5),str_len()-4));
					}
					else return "error";
				}
				else return "error";
			break;
		case 10:
			if((match_key(strs_crt->data)==24)&&(current_db!=NULL)&&(str_len()>=4))
				{
					int vi,j;
					strs *si;
					j=str_len();
					for(vi=0;vi<j;vi++)
					{
					    if(strlen(str_adr(vi+1)->data)>6)
					    {
					        char *vc;
                            vc=(char*)malloc (7);
                            strncpy(vc,str_adr(vi+1)->data,6);
                            vc[6]='\0';
                            if((strcmp(vc,"values")==0)&&(str_adr(vi+1)->data[6]=='('))
                            {
                                si=(strs*)malloc(sizeof(strs));
                                si->data =(char *)malloc(7);
                                strcpy(si->data,"values");
                                si->data[6]='\0';
                                si->last=str_adr(vi);
                                si->next=str_adr(vi+1);
                                (si->next)->last=si;
                                (si->last)->next=si;
                                strcpy(si->next->data,&(si->next->data[6]));
                            }
                            free(vc);
					    }

					}

					si=(strs *)malloc(sizeof(strs));
					si->data=(char *)malloc(7*sizeof(char));
					strcpy(si->data,"values");
					vi=match_str(strs_head,si);
					if(vi==4)
					{
						return insert_into(current_db->head_table,str_adr(3)->data,NULL,str_merge(str_adr(5),str_len()-vi));
					}
					else
					{
						if((vi>4)&&(vi<str_len()))
						return insert_into(current_db->head_table,str_adr(3)->data,str_merge(str_adr(4),vi-4),str_merge(str_adr(vi+1),str_len()-vi));
						else return "error";
					}
				}
				else return "error";
                break;
		case 11:
			if(strs_crt->next!=NULL)
			{
				strs_crt=strs_crt->next;
				switch (match_key(strs_crt->data))
                {
                    Db *qd;
                    Table *pt;
                    case 13:
                    qd=(Db*)malloc(sizeof(Db));
                    qd->head_table=NULL;
                    qd->name=strs_head->next->data;
                    qd->next=NULL;
                    Db *pd=ListLocateNode((void*)head_db,qd,db_equal,offsetof(struct db,next));
                    if(pd !=NULL)
                    {
                        if(str_len()==3)
                        {
                            return show_table(pd->head_table,0);
                        }
                        else
                        {
                            strs_crt=strs_crt->next;
                            if((str_len()==6) && (match_key(strs_crt->data)==18) &&  (match_key(strs_crt->next->data)==19))
                            {
                                strs_crt=strs_crt->next->next;
                                if(match_key(strs_crt->data)==20)
                                    return show_table(pd->head_table,DESC);
                                else
                                {
                                    if(match_key(strs_crt->data)==21)
                                        return show_table(pd->head_table,INCR);
                                    else return "error";
                                }
                            }
                            else return "error";
                        }
                    }
                    else return "error";
						break;
					case 17:
						pt=(struct table*)malloc(sizeof(struct table));
						pt->table_name=strs_head->next->data;
						Table *qt=ListLocateNode(current_db->head_table,pt,table_equal,offsetof(Table,next));
						if(qt!=NULL)
						{
							if(str_len()==3)
							{
								return show_column(qt->head_column,0);
							}
							else
							{
								strs_crt=strs_crt->next;
								if((str_len()==6) && (match_key(strs_crt->data)==18) &&  (match_key(strs_crt->next->data)==19))
								{
									strs_crt=strs_crt->next->next;
									if(match_key(strs_crt->data)==20)
										return show_column(qt->head_column,DESC);
									else
									{
										if(match_key(strs_crt->data)==21)
											return show_column(qt->head_column,INCR);
										else return "error";
									}
								}
								else return "error";
							}
						}
						else return "error";
						break;
					case 18:
						strs_crt=strs_crt->next;
						if((str_len()==5) && (match_key(strs_crt->data)==19))
						{
							strs_crt=strs_crt->next;
							if(match_key(strs_crt->data)==20)
							{
								if(match_key(strs_head->next->data)==16)
								{
									return show_db(head_db,DESC);
								}
								else
								{
									if(match_key(strs_head->next->data)==13)
									{
										if(current_db!=NULL)
										{
											return show_table(current_db->head_table,DESC);
										}
										else return "$";
									}
									else return "error";
								}
							}
							else
							{
								if(match_key(strs_crt->data)==21)
								{
									if(match_key(strs_head->next->data)==16)
									{
										return show_db(head_db,INCR);
									}
									else
									{
										if(match_key(strs_head->next->data)==13)
										{
											if(current_db!=NULL)
											{
												return show_table(current_db->head_table,INCR);
											}
											else return "$";
										}
										else return "error";
									}
								}
								else return "error";
							}
						}
						else return "error";
						break;
                    default:
                        return "error";
                        break;
					}
			}
			else
			{
				if (match_key(strs_crt->data)==16)
				{
					return show_db(head_db,0);
				}
				else
				{
					if(match_key(strs_crt->data)==13)
					{
						if(current_db!=NULL)
						{
							return show_table(current_db->head_table,0);
						}
						else return "$";
					}
					else return "error";
				}
			}
			break;

		default:
			return "error";
			break;
	}
	}
	else return "error";
	//show_strs();
	//strs_crt=strs_head->next;
	//printf("%s",str_merge(strs_crt,4));
	//printf("%s",str_tolower("FDSAfds,78%^&*a"));
	//printf("%d",match_key("DroP"));
	return NULL;
}

nums *num_test(int i)
{
	nums* nh,*ni;
	int j;
	j=0;
	nh=NULL;
	for(j=1;j<i+1;j++)
	{
		ni=(nums*)malloc(sizeof(nums));
		ni->data=j;
		ni->next=nh;
		nh=ni;
	}
	return nh;
}


char * update(struct table *ht,char *table_name,char *change,char *term)
{
	if(change[1]==' ')strcpy(&(change[1]),&(change[2]));
	Table *pt,*qt;
	pt=(struct table*)malloc(sizeof(struct table));
	pt->table_name=table_name;
	qt=ListLocateNode(ht,pt,table_equal,offsetof(Table,next));
	if(qt!=NULL)
	{
		nums *nh;
		nh=(nums *)solve_complex(qt,term);
        //nh=num_test(1);
		if (nh!=NULL)
		{
			char* col;
			col=str_bracket(change);
			col=strchar(col);
			//str_free(strs_head);
			strs_head=str_break(col);
			if(str_len()==1)
			{
				//str_free(strs_head);
				int i,j;
				j=0;
				if(col[0]==',')return "error";
				if(col[strlen(col)-1]==' ')
				{
				    col[strlen(col)-1]='\0';
                    j++;
				}
				//for(i=1;i<strlen(col);i++)if(col[i]==',')col[i]=' ';
				strs_head=strs_break(col);
				for(i=0;i<str_len();i++)
				{
					struct column *pc,*qc;
					pc=(struct column *)malloc(sizeof(struct column));
					pc->column_name=str_adr(i+1)->data;
					qc=ListLocateNode(qt->head_column,pc,column_equal,offsetof(Column,next));
					if(qc==NULL)return "error";
				}
				char* col1;
				col1=strchar(&(change[strlen(col)+2+j]));
				if(col1[0]=='=')
				{
					col1=strchar(&(col1[1]));
					if((col1[0]=='(')&&(col1[strlen(col1)-1]==')'))
					{
						strs *hs;
						char *col2;
						col2=str_bracket(col1);
						if((strlen(col2)+2)==strlen(col1))
						{
							col2=strchar(col2);
							if(col2[strlen(col2)-1]==' ')col2[strlen(col2)-1]='\0';
							hs=strs_break(col2);
							if(strs_len(hs)==strs_len(strs_head))
							{
								for(i=0;i<str_len();i++)
                                {
                                    struct column *pc,*qc;
                                    pc=(struct column *)malloc(sizeof(struct column));
                                    pc->column_name=str_adr(i+1)->data;
                                    qc=ListLocateNode(qt->head_column,pc,column_equal,offsetof(Column,next));
                                    if(qc->datatype==TEXT)
                                    {
                                        if((strs_adr(hs,i+1)->data)[0]=='"')(strs_adr(hs,i+1)->data)[0]='\'';
                                        if((strs_adr(hs,i+1)->data)[strlen((strs_adr(hs,i+1)->data))-1]=='"')(strs_adr(hs,i+1)->data)[strlen(strs_adr(hs,i+1)->data)-1]='\'';
                                        if((strs_adr(hs,i+1)->data)[0]!='\'')return "error";
                                        //printf("%s",strs_adr(hs,i+1)->data);
                                        char *str1=(char *)malloc(strlen(strs_adr(hs,i+1)->data)+1);
                                        strcpy(str1,&((strs_adr(hs,i+1)->data)[1]));
                                        str1[strlen(str1)-1]='\0';
                                        if(!is_text(str1))return "error";
                                    }
                                    if(qc->datatype!=TEXT)
                                    {
                                        if((strs_adr(hs,i+1)->data)[0]=='\'')return "error";
                                        if(qc->datatype==NONE) return "error";
                                        if(qc->datatype==INT)
                                        {
                                            if(!isint(strs_adr(hs,i+1)->data))return "error";
                                        }
                                        if(qc->datatype==FLOAT)
                                        {
                                            if((!isfloat(strs_adr(hs,i+1)->data))&&(!isint(strs_adr(hs,i+1)->data)))return "error";
                                            else  (strs_adr(hs,i+1)->data)=chfloat(strs_adr(hs,i+1)->data);
                                        }

                                    }
                                }

                                while(nh!=NULL)
								{
									for(i=0;i<str_len();i++)
									{
										struct column *pc,*qc;
										pc=(struct column*)malloc(sizeof(struct column));
										pc->column_name=str_adr(i+1)->data;
										qc=ListLocateNode(qt->head_column,pc,column_equal,offsetof(Column,next));
										//free((value_adr(qc->head_value,nh->data))->data);
										((value_adr(qc->head_value,nh->data))->data)=(char *)malloc(strlen(strs_adr(hs,i+1)->data));
										strcpy((value_adr(qc->head_value,nh->data))->data,strs_adr(hs,i+1)->data);
                                        //printf("%s",(strs_adr(hs,i+1)->data));
									}
									nh=nh->next;
								}
							}
							else return "error";
						}
						else return "error";
					}
					else return "error";
				}
				else return "error";


			}
			else return "error";
		}
		else return "error";

	}
	else return "error";
}

char * Delete(struct table *ht,char *table_name,char *term)
{
	struct table *qt,*pt;
	pt=(struct table*)malloc(sizeof(struct table));
	pt->table_name=table_name;
	qt=ListLocateNode(ht,pt,table_equal,offsetof(Table,next));
	if(qt!=NULL)
	{
		nums *nh;
		nh=(nums *)solve_complex(qt,term);
		//nh=num_test(1);
		if (nh!=NULL)
		{
			num_sort(nh,1);
			nums *ni;
			ni=nh;
			while (ni!=NULL)
			{
				val_d(qt->head_column,ni->data);
				ni=ni->next;

			}
			free(nh);
			return NULL;
		}
		else return "error";

	}
	else return "error";
}

