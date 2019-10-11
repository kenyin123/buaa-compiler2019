#pragma once
#include<stdio.h>
//#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

#define idlen 64
#define tab_size 512

FILE* fp_in;
FILE* fp_out;
char ch;//最新读取到的字符
int num_line = 1;//代码行数
char token[50000];//截取的一段，字符串或者数字等
char string_get[50000];//暂存字符串
int string_get_length;//暂存字符串长度
int int_get;//暂存数字
char char_get;//暂存char


int sym;//读取到的字符编号

char pro[13][20] = { "const","int","char","void","main","if","else","do","while","for","scanf","printf","return"
}; //保留字表

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
}symbol;

struct TAB {
	char idenfy[idlen];
	/*
	0:int常量
	1:char常量
	2:int变量
	3:char变量
	4:int数组
	5:char数组
	6:int函数
	7:char函数
	8:void函数
	9:int参数
	10:char参数
	*/
	int type;
	int value;//值或者长度
	int addr;//idenfy首地址或者相对偏移地址
	int size;//占用空间大小
	int lev;//作用域结构层次
};
struct TAB tab[tab_size];
int type = 0;
int value = 0;
int addr = 0;
int lev = 0;
int tab_location = 0;


void clear() {
	int i = 0;
	for (i = 0; i < sizeof(token); i++) {
		token[i] = '\0';
	}
	for (i = 0; i < sizeof(string_get); i++) {
		string_get[i] = '\0';
	}
}
int isSpace(char a) {
	if (a == ' ')return 1;
	else return 0;
}
int isNewline(char a) {
	if (a == '\n')return 1;
	else return 0;
}
int isTab(char a) {
	if (a == '\t')return 1;
	else return 0;
}
int isLetter(char a) {
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_')return 1;
	else return 0;
}
int isDigit(char a) {
	if (a >= '0' && a <= '9')return 1;
	else return 0;
}
int isSemi(char a) {
	if (a == ';')return 1;
	else return 0;
}
int isComma(char a) {
	if (a == ',')return 1;
	else return 0;
}
int isAssign(char a) {
	if (a == '=')return 1;
	else return 0;
}
int isPlus(char a) {
	if (a == '+')return 1;
	else return 0;
}
int isMinu(char a) {
	if (a == '-')return 1;
	else return 0;
}
int isDIV(char a) {
	if (a == '/')return 1;
	else return 0;
}
int isMULT(char a) {
	if (a == '*')return 1;
	else return 0;
}
int isLparent(char a) {
	if (a == '(')return 1;
	else return 0;
}
int isRparent(char a) {
	if (a == ')')return 1;
	else return 0;
}
int isLbrack(char a) {
	if (a == '[')return 1;
	else return 0;
}
int isRbrack(char a) {
	if (a == ']')return 1;
	else return 0;
}
int isLbrace(char a) {
	if (a == '{')return 1;
	else return 0;
}
int isRbrace(char a) {
	if (a == '}')return 1;
	else return 0;
}
int isLss(char a) {
	if (a == '<')return 1;
	else return 0;
}
int isGre(char a) {
	if (a == '>')return 1;
	else return 0;
}
int isQuotation(char a) {
	if (a == '\'')return 1;
	else return 0;
}
int isExclamation(char a) {
	if (a == '!')return 1;
	else return 0;
}

int isRelation(SYMBOL symbol) {
	if (symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ) {
		return 1;
	}
	return 0;
}

void catToken(char a) {//截取一串字符
	token[strlen(token)] = a;
}

void retract(char a) { //退回文件流
	ungetc(a, fp_in);
}
int reserver(char a[]) { //返回保留字识别码
	for (int i = 0; i < 13; i++) {
		if (strcmp(a, pro[i]) == 0) {
			return i + 5;
		}
	}
	return 0;
}
int transNum(char a[]) {
	return atoi(a);
}
void error(int i) {
	printf("error %d", num_line);
	exit(-1);
}

int getsym()
{
	ch = fgetc(fp_in);
	symbol = UNKNOWN;
	clear();
	while (isSpace(ch) || isTab(ch) || isNewline(ch)) {
		if (isSpace(ch)) {
			ch = fgetc(fp_in);
		}
		if (isTab(ch)) {
			ch = fgetc(fp_in);
		}
		if (isNewline(ch)) {
			num_line++;
			ch = fgetc(fp_in);
		}
	}
	if (isLetter(ch))
	{
		while (isLetter(ch) || isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp_in);
		}
		retract(ch);
		int resultValue = reserver(token);
		if (resultValue == 0)symbol = IDENFR; //返回0，没识别到，是标识符
		else symbol = (enum SYMBOL)resultValue; //保留字
	}
	else if (isDigit(ch))
	{
		if (ch == '0') {
			catToken(ch);
			symbol = INTCON;
			ch = fgetc(fp_in);
			if (isDigit(ch)) {
				error(0);
			}
			retract(ch);
			int_get = 0;
		}
		else {
			while (isDigit(ch))
			{
				catToken(ch);
				ch = fgetc(fp_in);
			}
			retract(ch);
			int_get = transNum(token);
			symbol = INTCON;
		}
	}
	else if (isPlus(ch)) symbol = PLUS;
	else if (isMinu(ch)) symbol = MINU;
	else if (isMULT(ch)) symbol = MULT;
	else if (isDIV(ch)) symbol = DIV;
	else if (isLparent(ch)) symbol = LPARENT;
	else if (isRparent(ch)) symbol = RPARENT;
	else if (isLbrack(ch)) symbol = LBRACK;
	else if (isRbrack(ch)) symbol = RBRACK;
	else if (isLbrace(ch)) symbol = LBRACE;
	else if (isRbrace(ch)) symbol = RBRACE;
	else if (isComma(ch)) symbol = COMMA;
	else if (isSemi(ch)) symbol = SEMICN;
	else if (isAssign(ch)) {
		ch = fgetc(fp_in);
		if (isAssign(ch)) symbol = EQL;
		else
		{
			symbol = ASSIGN;
			retract(ch);
		}
	}
	else if (isLss(ch)) {
		ch = fgetc(fp_in);
		if (isAssign(ch)) symbol = LEQ;
		else
		{
			retract(ch);
			symbol = LSS;
		}
	}
	else if (isGre(ch)) {
		ch = fgetc(fp_in);
		if (isAssign(ch)) symbol = GEQ;
		else
		{
			retract(ch);
			symbol = GRE;
		}
	}
	else if (ch == '\"') {
		ch = fgetc(fp_in);
		string_get_length = 0;
		while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
			string_get[string_get_length] = ch;
			string_get_length++;
			ch = fgetc(fp_in);
		}
		if (ch != '\"') error(0);
		symbol = STRCON;
	}
	else if (isQuotation(ch)) {
		ch = fgetc(fp_in);
		if (isDigit(ch) || isLetter(ch) || (ch == '*') || (ch == '/') || (ch == '+') || (ch == '-'))symbol = CHARCON;
		else error(0);
		char_get = ch;
		ch = fgetc(fp_in);
		if (ch != '\'')error(0);
	}
	else if (isExclamation(ch)) {
		ch = fgetc(fp_in);
		if (isAssign(ch)) {
			symbol = NEQ;
		}
	}
	else if (ch == EOF) {
		symbol = UNKNOWN;
	}
	else error(0);
	switch ((int)symbol) {
	case 0:printf("UNKNOWN\n"); break;
	case 1:printf("IDENFR %s\n", token); break;
	case 2:printf("INTCON %d\n", int_get); break;
	case 3:printf("CHARCON %c\n", char_get); break;
	case 4:printf("STRCON %s\n", string_get); break;
	case 5:printf("CONSTTK const\n"); break;
	case 6:printf("INTTK int\n"); break;
	case 7:printf("CHARTK char\n"); break;
	case 8:printf("VOIDTK void\n"); break;
	case 9:printf("MAINTK main\n"); break;
	case 10:printf("IFTK if\n"); break;
	case 11:printf("ELSETK else\n"); break;
	case 12:printf("DOTK do\n"); break;
	case 13:printf("WHILETK while\n"); break;
	case 14:printf("FORTK for\n"); break;
	case 15:printf("SCANFTK scanf\n"); break;
	case 16:printf("PRINTFTK printf\n"); break;
	case 17:printf("RETURNTK return\n"); break;
	case 18:printf("PLUS +\n"); break;
	case 19:printf("MINU -\n"); break;
	case 20:printf("MULT *\n"); break;
	case 21:printf("DIV /\n"); break;
	case 22:printf("LSS <\n"); break;
	case 23:printf("LEQ <=\n"); break;
	case 24:printf("GRE >\n"); break;
	case 25:printf("GEQ >=\n"); break;
	case 26:printf("EQL ==\n"); break;
	case 27:printf("NEQ !=\n"); break;
	case 28:printf("ASSIGN =\n"); break;
	case 29:printf("SEMICN ;\n"); break;
	case 30:printf("COMMA ,\n"); break;
	case 31:printf("LPARENT (\n"); break;
	case 32:printf("RPARENT )\n"); break;
	case 33:printf("LBRACK [\n"); break;
	case 34:printf("RBRACK ]\n"); break;
	case 35:printf("LBRACE {\n"); break;
	case 36:printf("RBRACE }\n"); break;
	}
	return 0;
}


int Grammar_unsigned_int(); 
int Grammar_int();
int Const_Definition();
int Const_Declaration();
int Var_Definition();
int Var_Declaration();

int Factor();
int Term();
int Expression();

void paramHandler();

void statementHandler();
void statement_list();
void complex_statement();

void condition();
void if_Handler();
void while_Handler();
void do_while_Handler();
int step_length();
void for_Handler();
void value_param_list();
void call_fun_Handler();
void assign_Handler();
void scanf_Handler();
void printf_Handler();
void return_Handler();
//void empty_Handler();
void returned_func_definition();
int unreturn_func_definition();
void mainfunc();
void program();



/*
void entertab(char* idenfy, int type, int value, int addr, int lev);
int searchtab(char* idenfy, int filed);
void printtab();
*/
