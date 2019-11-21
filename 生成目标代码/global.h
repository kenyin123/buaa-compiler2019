#pragma once

#define idlen 1024//接收标识符的有效长度
#define tab_size 1024//符号表最大长度
#define token_len 1024//token数组最大长度
#define midecode_size 4096

//符号表项类型
#define const_int 0
#define const_char 1
#define var_int 2
#define var_char 3
#define array_int 4
#define array_char 5
#define return_int_func 6
#define return_char_func 7
#define void_func 8
#define int_para 9
#define char_para 10

#define return_array_len 1024//错误处理中间检查函数返回值的数组长度

//中间代码的类型
#define CONST_INT 0
#define CONST_CHAR 1
#define VAR_INT 2
#define VAR_CHAR 3
#define ARRAY_INT 4
#define ARRAY_CHAR 5

#define PLUSOP 6
#define MINUOP 7
#define MULTOP 8
#define DIVOP 9

#define FUNC_INT 10
#define FUNC_CHAR 11
#define FUNC_VOID 12
#define PARA_INT 13
#define PARA_CHAR 14
#define CALL 15
#define PUSH 16
#define FUNC_RET 17

#define VAR_ASSIGN 18
#define ARRAY_ASSIGN 19
#define ARRAY_ACCESS 20 //a = array[i]

#define LSSOP 21
#define LEQOP 22
#define GREOP 23
#define GEQOP 24
#define EQLOP 25
#define NEQOP 26

#define SETLABEL 27//set label
#define GOTO 28
#define BNZ 29
#define BZ 30

#define SCANF_INT 31
#define SCANF_CHAR 32
#define PRINT_INT 33
#define PRINT_CHAR 34
#define PRINT_STR 35

#define RET_EXPR 36
#define RET_NULL 37

#define DEL 38//?
#define PRINT_NEWLINE 39



#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
extern FILE* fp_in;
extern FILE* grammar_out;
extern FILE* error_out;
extern FILE* tab_out;
extern FILE* midcode_out;

extern char ch;//最新读取到的字符
extern int num_line;//代码行数
extern char token[token_len];//截取的一段，字符串或者数字等
extern int int_get;//int类型数字
extern int num_func;//函数个数
//extern char ident[token_len];//暂时存储函数名字，用于函数调用
extern int expr_is_char;//判断表达式是不是单个字符

extern char pro[13][20];

enum SYMBOL {
	UNKNOWN = 0,
	IDENFR = 1,//标识符
	INTCON = 2,//整型常量
	CHARCON = 3,//字符常量
	STRCON = 4,//字符串
	CONSTTK = 5,//const
	INTTK = 6,//int
	CHARTK = 7,//char
	VOIDTK = 8,//void
	MAINTK = 9,//main
	IFTK = 10,//if
	ELSETK = 11,//else
	DOTK = 12,//do
	WHILETK = 13,//while
	FORTK = 14,//for
	SCANFTK = 15,//scanf
	PRINTFTK = 16,//printf
	RETURNTK = 17,//return
	PLUS = 18,//+
	MINU = 19,//-.
	MULT = 20,//*
	DIV = 21,//'/'
	LSS = 22,//<
	LEQ = 23,//<=
	GRE = 24,//>
	GEQ = 25,//>=
	EQL = 26,//==
	NEQ = 27,//!=
	ASSIGN = 28,// =
	SEMICN = 29,//;
	COMMA = 30,//,
	LPARENT = 31,//(
	RPARENT = 32,//)
	LBRACK = 33,//[
	RBRACK = 34,//]
	LBRACE = 35,//{
	RBRACE = 36,//}
};
extern enum SYMBOL symbol;

//词法分析阶段函数定义
void clear();
int isNewline(char a);
int isLetter(char a);
int isDigit(char a);
int isSemi(char a);
int isComma(char a);
int isAssign(char a);
int isPlus(char a);
int isMinu(char a);
int isDIV(char a);
int isMULT(char a);
int isLparent(char a);
int isRparent(char a);
int isLbrack(char a);
int isRbrack(char a);
int isLbrace(char a);
int isRbrace(char a);
int isLss(char a);
int isGre(char a);
int isQuotation(char a);
int isExclamation(char a);
int isRelation(enum SYMBOL symbol);
void catToken(char a);
void retract(char a);
int reserver(char a[]);
int transNum(char a[]);
int getsym(int flag);
void print_symbol(enum SYMBOL sym);



//语法分析阶段函数定义
int Grammar_unsigned_int();
int Grammar_int();
int Const_Definition(int field_flag);
int Const_Declaration(int field_flag);
int Var_Declaration(int field_flag);
int Factor();
int Term();
int Expression();
void paramHandler();
int statementHandler();
void statement_list();
void complex_statement(int field_flag);
void condition();
void if_Handler();
void while_Handler();
void do_while_Handler();
int step_length();
void for_Handler();
void value_param_list(int j);
void call_fun_Handler(char* token_temp);
void assign_Handler(char* token_temp);
void scanf_Handler();
void printf_Handler();
void return_Handler();
void returned_func_definition();
int unreturn_func_definition();
void mainfunc();
void program();



//只用在变量说明阶段的单向链表
struct sym_str {
	int symbol;
	char token[token_len];
	struct sym_str* next_symbol;
};
typedef struct sym_str sym_str;
sym_str* enter_sym_str(sym_str* p);
void printf_sym_str(sym_str* p);

//符号表
struct str_tab {
	char id[idlen];
	/*
	符号表项类型
	#define const_int 0;
	#define const_char 1;
	#define var_int 2;
	#define var_char 3;
	#define array_int 4;
	#define array_char 5;
	#define return_int_func 6;
	#define return_char_func 7;
	#define void_func 8;
	#define int_para 9;
	#define char_para 10;
	*/
	int type;
	int value;
	int addr;
	int size;
	int lev;
};
extern int type;
extern int value;
extern int addr;
extern int lev;
extern struct str_tab tab[tab_size];
extern int tab_loc;//当前在tab中间的位置,即符号表长度

void str_cpy(char* str1, char* str2);
void entertab(char* id, int type, int value, int addr, int lev);
int searchtab(char* ident,int num_func);
void printtab();

//错误处理部分
void skip();
/*
注意program里面可能有其他错误
0      //未分类错误
a 1    //非法符号或不符合词法
b 2    //名字重定义
c 3    //未定义的名字
d 4    //函数参数个数不匹配
e 5    //函数参数类型不匹配
f 6    //条件判断中出现不合法的类型
g 7    //无返回值的函数存在不匹配的return语句
h 8    //有返回值的函数缺少return语句或存在不匹配的return语句×还有一个地方，递归
i 9    //数组元素的下标只能是整型表达式 因子和赋值语句
j 10    //不能改变常量的值
k 11    //应为分号√
l 12    //应为)√
m 13    //应为]√
n 14    //do-while语句缺少while√
o 15    //常量定义中 = 后面只能是整型或字符型常量× 全局错误的话会报异常
*/
void error(int i);


/*
error_gh
0:int
1:char
2:return;
*/
extern char return_array[return_array_len];
void judge_no_return(enum SYMBOL sym);
void judge_error_return(enum SYMBOL sym, int type);
extern enum SYMBOL symbol_error_gh;
//error_klmn
extern int num_line_temp;
extern int error_klmn;
extern enum SYMBOL symbol_error_klmn;
//error_a
extern int error_a;
extern char ch_temp;


//中间代码部分
struct str_midcode {
	int type;
	char argu1[idlen];
	char argu2[idlen];
	char result[idlen];
	int value;//常量的值或者数组的长度
};
extern struct str_midcode midcode[midecode_size];
extern int midcode_loc;
extern int id_name_num;
extern int label_name_num;//用于标签起名
void insert_midcode(int type,char* argu1,char* argu2,char* result,int value);
char* id_name_gen();
void print_midcode();
char* label_name_gen();
char* numtostr(int num);