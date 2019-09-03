#pragma warning(disable:4996)
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
FILE *fp;
char token[100];//截取的一段
char string1[500];//字符串 
char ch;//last char
int sym;//读取到的字符编号
char pro[13][20] = {"const","int","char","void","main","if","while","swicth","case","default","scanf","printf","return"
}; //保留字表

enum SYMBOL {
	PLUS = 1,//+
	MINUS = 2,//-
	STAR = 3,//*
	DIVI = 4,//'/'
	COLON = 5,//:
	SEMI = 6,//;
	COMMA = 7,//,
	LPAR = 8,//(
	RPAR = 9,//)
	LMPAR = 10,//[
	RMPAR = 11,//]
	LBRACE = 12,//{
	RBRACE = 13,//}
	LAB = 14,//<
	RAB = 15,//>
	ASSIGN = 16,// =
	NLT = 17,//>=
	NMT = 18,//<=
	NEQU = 19,//!=
	EQU = 20,//==
	ID = 21,//标识符
	INT = 22,
	STRING = 23,
	CHARACTER = 24,

	CONST = 30,
	INTSY = 31,
	CHARSY = 32,
	VOIDSY = 33,
	MAINSY = 34,
	IFSY = 35,
	WHILESY = 36,
	SWITCHSY = 37,
	CASESY = 38,
	DEFAULTSY = 39,
	SCANFSY = 40,
	PRINTFSY = 41,
	RETURN = 42,
	
}symbol;

void clearToken() {
	int i = 0;
	for (i = 0; i < sizeof(token); i++) {
		token[i] = '\0';
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
	if ((a >= 'a'&&a <= 'z') || (a >= 'A' && a <= 'Z')|| a == '_' )return 1;
	else return 0;
}
int isDigit(char a) {
	if (a >= '0'&&a <= '9')return 1;
	else return 0;
}
int isColon(char a) {
	if (a == ':')return 1;
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
int isEqu(char a) {
	if (a == '=')return 1;
	else return 0;
}
int isPlus(char a) {
	if (a == '+')return 1;
	else return 0;
}
int isMinus(char a) {
	if (a == '-')return 1;
	else return 0;
}
int isDivi(char a) {
	if (a == '/')return 1;
	else return 0;
}
int isStar(char a) {
	if (a == '*')return 1;
	else return 0;
}
int isLpar(char a) {
	if (a == '(')return 1;
	else return 0;
}
int isRpar(char a) {
	if (a == ')')return 1;
	else return 0;
}
int isLmpar(char a) {
	if (a == '[')return 1;
	else return 0;
}
int isRmpar(char a) {
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
int isLab(char a) {
	if (a == '<')return 1;
	else return 0;
}
int isRab(char a) {
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
	for (int i = 0; i < sizeof(pro); i++) {
		if (strcmp(a, pro[i]) == 0) {
			return i + 30;
		}
	}
	return 0;
}
int transNum(char a[]) {
	return atoi(a);
}
void error(int i) {
	printf("error %n",i);
}

int getsym()
{
	clearToken();
	while (isSpace(ch) || isNewline(ch) || isTab(ch))
		ch = fgetc(fp);									
	if (isLetter(ch))
	{
		while (isLetter(ch) || isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp);
		}
		retract(ch);
		int resultValue = reserver(token);
		if (resultValue == 0)symbol = ID; //返回0，没识别到，是标识符
		else symbol = (enum SYMBOL)resultValue; //保留字
	}
	else if (isDigit(ch))
	{
		while (isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp);
		}
		retract(ch);
		symbol = INTSY;
	}
	else if (isColon(ch))symbol = COLON;
	else if (isPlus(ch)) symbol = PLUS;
	else if (isMinus(ch)) symbol = MINUS;
	else if (isStar(ch)) symbol = STAR;
	else if (isDivi(ch)) symbol = DIVI;
	else if (isLpar(ch)) symbol = LPAR;
	else if (isRpar(ch)) symbol = RPAR;
	else if (isLmpar(ch)) symbol = LMPAR;
	else if (isRmpar(ch)) symbol = RMPAR;
	else if (isLbrace(ch)) symbol = LBRACE;
	else if (isRbrace(ch)) symbol = RBRACE;
	else if (isComma(ch)) symbol = COMMA;
	else if (isSemi(ch)) symbol = SEMI;
	else if (isEqu(ch)) {
		ch = fgetc(fp);
		if (isEqu(ch)) symbol = EQU;
		else
		{
			symbol = ASSIGN;
			retract(ch);
		}
	} 
	else if (isLab(ch)) {
		ch = fgetc(fp);
		if (isEqu(ch)) symbol = NMT;
		else
		{
			retract(ch);
			symbol = LAB;
		}
	}
	else if (isRab(ch)) {
		ch = fgetc(fp);
		if (isEqu(ch)) symbol = NLT;
		else
		{
			retract(ch);
			symbol = RAB;
		}
	}
	else if (ch == '\"') {
		ch = fgetc(fp);
		while (ch == 32 || ch == 33 || ch >= 35 && ch <= 126) {
			token[strlen(token)] = ch;
			ch = fgetc(fp);
		}
		if (ch != '\"') error(0);
		symbol = STRING;
	}
	else if (isQuotation(ch)) {
		ch = fgetc(fp);
		if (isDigit(ch)|| isLetter(ch)|| (ch == '*') || (ch == '/') || (ch == '+') || (ch == '-'))symbol = CHARACTER;
		else error(0);
		ch = fgetc(fp);
		if (ch != '\'')error(0);
	}
	else if (isExclamation(ch)) {
		ch = fgetc(fp);
		if (isEqu(ch)) {
			symbol = NEQU;
		}
	}
	else error(0);
	//printf("%d\n", (int)symbol);
	return 0;
}
int main() {
	fp = fopen("1.txt", "r");
	ch = fgetc(fp);
	while (ch != EOF) {
		getsym();
		sym = (int)symbol;
		printf("%d ", sym);
		switch (sym) {
		case 1:printf("PLUS +"); break;
		case 2:printf("MINUS -"); break;
		case 3:printf("STAR *"); break;
		case 4:printf("DIVI /"); break;
		case 5:printf("colon :"); break;
		case 6:printf("SEMI ;"); break;
		case 7:printf("COMMA ,"); break;
		case 8:printf("LPAR )"); break;
		case 9:printf("RPAR)"); break;
		case 10:printf("LMPAR ["); break;
		case 11:printf("RMPAR ]"); break;
		case 12:printf("{"); break;
		case 13:printf("}"); break;
		case 14:printf("LAB <"); break;
		case 15:printf("RAB >"); break;
		case 16:printf("EQU ="); break;
		case 17:printf("NLT >="); break;
		case 18:printf("NMT <="); break;
		case 19:printf("NEQU !>"); break;
		case 20:printf("ASSIGN =="); break;
		case 21:printf("symbol %s", token); break;
		case 22:printf("integer %s", token); break;
		case 23:printf("string %s", token); break;
		case 24:printf("char"); break;
		default:printf("else");
		}
		printf("\n");
		ch = fgetc(fp);
	}
	system("pause");
	return 0;

}
