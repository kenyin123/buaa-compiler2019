#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>


char token[100];
char string1[500];//×Ö·û´® 
char ch;//last char
int num, i;
float real;
int sym;
char pro[22][20] = { "begin","end","if","then","else","const","var","array","of","integer",
"real","char","procedure","function","repeat","until","for","to","downto","do","read","write"
}; //±£Áô×Ö±í
enum SYMBOL {
	POINT = 0,
	PLUS = 1,
	MINUS = 2,
	STAR = 3,
	DIVI = 4,
	LPAR = 5,
	RPAR = 6,
	COLON = 7,
	SEMI = 8,
	ASSIGN = 9,
	LMPAR = 10,
	RMPAR = 11,
	COMMA = 12,
	LAB = 13,
	RAB = 14,
	EQU = 15,
	NLT = 16,
	NMT = 17,
	NEQU = 18,
	ID = 19,
	INT = 20,
	REAL = 21,
	STRING = 22,
	resultValue
}symbol;

void clearToken() {
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
	if ((a >= 'a'&&a <= 'z') || (a >= 'A') && a <= 'Z')return 1;
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
int isComma(char a) {
	if (a == ',')return 1;
	else return 0;
}
int isSemi(char a) {
	if (a == ';')return 1;
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
int isPoint(char a) {
	if (a == '.')return 1;
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
int isLab(char a) {
	if (a == '<')return 1;
	else return 0;
}
int isRab(char a) {
	if (a == '>')return 1;
	else return 0;
}



void catToken(char a) {
	token[strlen(token)] = a;
}
void retract(char a) {
	ungetc(a, stdin);
}
int reserver(char a[]) {
	for (i = 0; i < sizeof(pro); i++) {
		if (strcmp(a, pro[i]) == 0) {
			return i + 1 + 22;
		}
	}
	return 0;
}
int transNum(char a[]) {
	return atoi(a);
}
double transreal(char a[]) {
	return atof(a);
}



void error() {
	printf("error!");
}


int getsym()
{
	clearToken();
	while (isSpace(ch) || isNewline(ch) || isTab(ch))
		ch = getchar();									//Ìø¹ý¿Õ¸ñ¡¢»»ÐÐ¡¢tab 
	if (isLetter(ch))
	{
		while (isLetter(ch) || isDigit(ch))
		{
			catToken(ch);
			ch = getchar();
		}
		retract(ch);
		int resultValue = reserver(token);
		if (resultValue == 0)symbol = ID;
		else symbol = (enum SYMBOL)resultValue;
	}
	else if (isDigit(ch))
	{
		int point_time = 0;
		while (isDigit(ch) || isPoint(ch))
		{
			if (isDigit(ch)) {
				catToken(ch);
				ch = getchar();
			}
			if (isPoint(ch)) {
				if (point_time == 0) {
					catToken(ch);
					ch = getchar();
					point_time++;
				}
				else error();
			}
		}
		retract(ch);

		if (point_time == 0) {
			// num = transNum(token);
			symbol = INT;
		}
		if (point_time == 1) {
			// real = transreal(token);
			symbol = REAL;
		}

	}
	else if (isColon(ch))
	{
		ch = getchar();
		if (isEqu(ch)) symbol = ASSIGN;
		else
		{
			retract(ch);
			symbol = COLON;
		}
	}
	else if (isPlus(ch)) symbol = PLUS;
	else if (isMinus(ch)) symbol = MINUS;
	else if (isStar(ch)) symbol = STAR;
	else if (isDivi(ch)) symbol = DIVI;
	else if (isLpar(ch)) symbol = LPAR;
	else if (isRpar(ch)) symbol = RPAR;
	else if (isComma(ch)) symbol = COMMA;
	else if (isSemi(ch)) symbol = SEMI;
	else if (isEqu(ch)) symbol = EQU;
	else if (isPoint(ch)) symbol = POINT;
	else if (isLmpar(ch)) symbol = LMPAR;
	else if (isRmpar(ch)) symbol = RMPAR;
	else if (isLab(ch)) {
		ch = getchar();
		if (isEqu(ch)) symbol = NMT;
		else if (isRab(ch)) symbol = NEQU;
		else
		{
			retract(ch);
			symbol = LAB;
		}
	}
	else if (isRab(ch)) {
		ch = getchar();
		if (isEqu(ch)) symbol = NLT;
		else
		{
			retract(ch);
			symbol = RAB;
		}
	}
	else if (ch == '\"') {
		ch = getchar();
		while (ch == 32 || ch == 33 || ch >= 35 && ch <= 126) {
			token[strlen(token)] = ch;
			ch = getchar();
		}
		if (ch != '\"') error();
		symbol = STRING;
	}
	else error();
	return 0;
}

int main() {
	ch = getchar();
	while (!isPoint(ch)) {
		getsym();
		sym = (int)symbol;
		printf("%d ", sym);
		switch (sym) {
		case 1:printf("PLUS +"); break;
		case 2:printf("MINUS -"); break;
		case 3:printf("STAR *"); break;
		case 4:printf("DIVI /"); break;
		case 5:printf("LPAR ("); break;
		case 6:printf("RPAR )"); break;
		case 7:printf("COLON :"); break;
		case 8:printf("SEMI ;"); break;
		case 9:printf("ASSIGN :="); break;
		case 10:printf("LMPAR ["); break;
		case 11:printf("RMPAR ]"); break;
		case 12:printf("COMMA ,"); break;
		case 13:printf("LAB <"); break;
		case 14:printf("RAB >"); break;
		case 15:printf("EQU ="); break;
		case 16:printf("NLT >="); break;
		case 17:printf("NMT <="); break;
		case 18:printf("NEQU <>"); break;
		case 19:printf("symbol %s", token); break;
		case 20:printf("integer %s", token); break;
		case 21:printf("real %s", token); break;
		case 22:printf("string %s", token); break;
		case 23:printf("begin"); break;
		case 24:printf("end"); break;
		case 25:printf("if"); break;
		case 26:printf("then"); break;
		case 27:printf("else"); break;
		case 28:printf("const"); break;
		case 29:printf("var"); break;
		case 30:printf("array"); break;
		case 31:printf("of"); break;
		case 32:printf("integer"); break;
		case 33:printf("real"); break;
		case 34:printf("char"); break;
		case 35:printf("procedure"); break;
		case 36:printf("function"); break;
		case 37:printf("repeat"); break;
		case 38:printf("until"); break;
		case 39:printf("for"); break;
		case 40:printf("to"); break;
		case 41:printf("downto"); break;
		case 42:printf("do"); break;
		case 43:printf("read"); break;
		case 44:printf("write"); break;
		}
		printf("\n");
		ch = getchar();
	}
	return 0;

}
