#pragma once
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

FILE* fp;
char ch;//最新读取到的字符
int num_line = 1;//代码行数
char token[100];//截取的一段，字符串或者数字等

char string_get[500];//暂存字符串
int string_get_length;//暂存字符串长度
int int_get;//暂存数字
char char_get;//暂存char


int sym;//读取到的字符编号

char pro[13][20] = { "const","int","char","void","main","if","else","do","while","for","scanf","printf","return"
}; //保留字表

enum SYMBOL {
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

	SPACE = 40,
	NEWLINE = 41,
	TAB = 42
}symbol;

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

void catToken(char a) {//截取一串字符
	token[strlen(token)] = a;
}

void retract(char a) { //退回文件流
	ungetc(a, fp);
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
	printf("error %d", i);
}

int getsym();
int getsym()
{
	clear();
	if (isSpace(ch)) {
		symbol = SPACE;
		return 0;
	}
	if (isTab(ch)) {
		symbol = TAB;
		return 0;
	}
	if (isNewline(ch)) {
		symbol = NEWLINE;
		num_line++;
		return 0;
	}
	if (isLetter(ch))
	{
		while (isLetter(ch) || isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp);
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
			ch = fgetc(fp);
			if (isDigit(ch)) {
				error(0);
			}
			retract(ch);
			int_get = 0;
			return 0;
		}
		while (isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp);
		}
		retract(ch);
		int_get = transNum(token);
		symbol = INTCON;
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
		ch = fgetc(fp);
		if (isAssign(ch)) symbol = EQL;
		else
		{
			symbol = ASSIGN;
			retract(ch);
		}
	}
	else if (isLss(ch)) {
		ch = fgetc(fp);
		if (isAssign(ch)) symbol = LEQ;
		else
		{
			retract(ch);
			symbol = LSS;
		}
	}
	else if (isGre(ch)) {
		ch = fgetc(fp);
		if (isAssign(ch)) symbol = GEQ;
		else
		{
			retract(ch);
			symbol = GRE;
		}
	}
	else if (ch == '\"') {
		ch = fgetc(fp);
		string_get_length = 0;
		while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
			string_get[string_get_length] = ch;
			string_get_length++;
			ch = fgetc(fp);
		}
		if (ch != '\"') error(0);
		symbol = STRCON;
	}
	else if (isQuotation(ch)) {
		ch = fgetc(fp);
		if (isDigit(ch) || isLetter(ch) || (ch == '*') || (ch == '/') || (ch == '+') || (ch == '-'))symbol = CHARCON;
		else error(0);
		char_get = ch;
		ch = fgetc(fp);
		if (ch != '\'')error(0);
	}
	else if (isExclamation(ch)) {
		ch = fgetc(fp);
		if (isAssign(ch)) {
			symbol = NEQ;
		}
	}
	else error(0);
	//printf("%d\n", (int)symbol);
	return 0;
}

/*
int Check_int();
int Const_Definition();
int Const_Declaration();
int Var_Declaration();
int Factor();
int Term();
int Expression();

*/
int Returned_Fun();
int Unreturned_Fun();




