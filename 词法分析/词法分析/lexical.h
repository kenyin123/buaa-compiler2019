#pragma once
#include<stdio.h>
//#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

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
	IDENFR = 1,//标识符
	INTCON = 2,//整型常量
	CHARCON = 3,//字符常量
	STRCON = 4,//字符串
	CONSTTK =5,//const
	INTTK = 6,//int
	CHARTK = 7,//char
	VOIDTK = 8,//void
	MAINTK =9,//main
	IFTK = 10,//if
	ELSETK =11,//else
	DOTK =12,//do
	WHILETK =13,//while
	FORTK =14,//for
	SCANFTK =15,//scanf
	PRINTFTK =16,//printf
	RETURNTK = 17,//return
	PLUS = 18,//+
	MINU = 19,//-.
	MULT = 20,//*
	DIV = 21,//'/'
	LSS = 22,//<
	LEQ = 23,//<=
	GRE = 24,//>
	GEQ =25,//>=
	EQL =26,//==
	NEQ =27,//!=
	ASSIGN =28,// =
	SEMICN = 29,//;
	COMMA = 30,//,
	LPARENT = 31,//(
	RPARENT =32,//)
	LBRACK = 33,//[
	RBRACK =34,//]
	LBRACE =35,//{
	RBRACE =36,//}
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
	//printf("error %d", i);
}

int getsym();




