/*************************************************************************
    > File Name: calc.h
    > Author: hao
    > Mail: onerhao@gmail.com
    > Created Time: Thu 25 Oct 2012 01:43:57 PM CST
 ************************************************************************/

#define MAX 256

extern int valid_expression(char *infix);
//validate a math expression,check errors

extern int fisi(double x);
//verify whether a float number x is integer(with .000);

extern double calc_num(char *infix,double *result);
//to calculate a math expression's value;

extern double plus(double s,double t);
extern double subduct(double s,double t);
extern double divide(double s,double t);
extern double multiply(double s,double t);
extern double module(double s);
extern double binary(double s,char c,double t);
//the binary operation

extern double unary(double s,char c);
extern int comp_num(double x,double y,char *optr);
//compare two numbers' relationship(bigger or smaller)

