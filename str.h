#ifndef STR_H
#define STR_H


typedef struct strs
{
	char *data;
	struct strs *last;
	struct strs *next;
}strs;


typedef struct nums
{
	int data;
	struct nums *next;
}nums;

extern strs *strs_head;
extern strs *strs_crt;
extern nums *nums_head;
extern nums *nums_crt;


strs *str_break(char *str);//新增将关键字都小写
strs *strs_break(char *str);
char *str_merge(strs *hstr,int n);//将逗号合并了，无空格，不过可能出现两个逗号连接的情况
int str_len();
int strs_len(strs *hs);
strs *str_adr(int n);//输入第几个（从1开始），输出结点的地址,超出输出NULL
strs *strs_adr(strs *hs,int n);
void str_std(char *str);
char *str_tolower(char *str);
char *str_bracket(char *str);//输入带括号的表达式，返回第一个括号内两头不带括号的内容,输入的字符串第一个必须为'('。

int match_key(char *str);

nums *num_and(nums *n1,nums *n2);
nums *num_or(nums *n1,nums *n2);
int num_len(nums *nh);
void num_sort(nums *nh,int order);//0为升序，1为降序
nums *num_adr(nums *nh,int n);//与str_adr同理
int match_num(nums *nh,nums *ni);//返回ni在nh链中的位置（从1开始），不存在则返回0；

int match_str(strs *sh,strs *si);//同理

int str_pos(char *str,char c,int t);//返回c在text中第t次出现的位置(从0开始)，t>0,不存在返回-1

int wildcard(const char *text, const char *wi, int flag);//通配符，初始flag=0；前面输column 后面是带*或？的字符串		返回0匹配

//void val_d(struct column *hc,int nc);//删整行数据

struct value *value_adr(struct value *vh,int n);

int is_str(char *src);
int is_text(char *src);
char * chfloat(char *str1);

#endif
