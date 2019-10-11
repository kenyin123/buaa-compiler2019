#pragma warning(disable:4996)
#include<stdio.h>
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
	printf("error %d\n", num_line);
}

int getsym()
{
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
			return 0;
		}
		while (isDigit(ch))
		{
			catToken(ch);
			ch = fgetc(fp_in);
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
		else error(0);
	}
	else if (ch == EOF) {
		symbol = UNKNOWN;
	}
	else error(0);
	return 0;
}
int main() {
	fp_in = fopen("testfile.txt", "r");
	fp_out = fopen("output.txt", "w");
	ch = fgetc(fp_in);
	while (ch != EOF) {
		getsym();
		sym = (int)symbol;
		switch (sym) {
		/*
		case 0:break;
		case 1:fprintf(fp_out, "\nIDENFR %s", token); break;
		case 2:fprintf(fp_out, "\nINTCON %d", int_get); break;
		case 3:fprintf(fp_out, "\nCHARCON %c", char_get); break;
		case 4:fprintf(fp_out, "\nSTRCON %s", string_get); break;
		case 5:fprintf(fp_out, "\nCONSTTK const"); break;
		case 6:fprintf(fp_out, "\nINTTK int"); break;
		case 7:fprintf(fp_out, "\nCHARTK char"); break;
		case 8:fprintf(fp_out, "\nVOIDTK void"); break;
		case 9:fprintf(fp_out, "\nMAINTK main"); break;
		case 10:fprintf(fp_out, "\nIFTK if"); break;
		case 11:fprintf(fp_out, "\nELSETK else"); break;
		case 12:fprintf(fp_out, "\nDOTK do"); break;
		case 13:fprintf(fp_out, "\nWHILETK while"); break;
		case 14:fprintf(fp_out, "\nFORTK for"); break;
		case 15:fprintf(fp_out, "\nSCANFTK scanf"); break;
		case 16:fprintf(fp_out, "\nPRINTFTK printf"); break;
		case 17:fprintf(fp_out, "\nRETURNTK return"); break;
		case 18:fprintf(fp_out, "\nPLUS +"); break;
		case 19:fprintf(fp_out, "\nMINU -"); break;
		case 20:fprintf(fp_out, "\nMULT *"); break;
		case 21:fprintf(fp_out, "\nDIV /"); break;
		case 22:fprintf(fp_out, "\nLSS <"); break;
		case 23:fprintf(fp_out, "\nLEQ <="); break;
		case 24:fprintf(fp_out, "\nGRE >"); break;
		case 25:fprintf(fp_out, "\nGEQ >="); break;
		case 26:fprintf(fp_out, "\nEQL =="); break;
		case 27:fprintf(fp_out, "\nNEQ !="); break;
		case 28:fprintf(fp_out, "\nASSIGN ="); break;
		case 29:fprintf(fp_out, "\nSEMICN ;"); break;
		case 30:fprintf(fp_out, "\nCOMMA ,"); break;
		case 31:fprintf(fp_out, "\nLPARENT ("); break;
		case 32:fprintf(fp_out, "\nRPARENT )"); break;
		case 33:fprintf(fp_out, "\nLBRACK ["); break;
		case 34:fprintf(fp_out, "\nRBRACK ]"); break;
		case 35:fprintf(fp_out, "\nLBRACE {"); break;
		case 36:fprintf(fp_out, "\nRBRACE }"); break;
		
		*/
		case 0:break;
		case 1:printf("\nIDENFR %s", token); break;
		case 2:printf("\nINTCON %d", int_get); break;
		case 3:printf("\nCHARCON %c", char_get); break;
		case 4:printf("\nSTRCON %s", string_get); break;
		case 5:printf("\nCONSTTK const"); break;
		case 6:printf("\nINTTK int"); break;
		case 7:printf( "\nCHARTK char"); break;
		case 8:printf( "\nVOIDTK void"); break;
		case 9:printf( "\nMAINTK main"); break;
		case 10:printf( "\nIFTK if"); break;
		case 11:printf( "\nELSETK else"); break;
		case 12:printf( "\nDOTK do"); break;
		case 13:printf( "\nWHILETK while"); break;
		case 14:printf( "\nFORTK for"); break;
		case 15:printf( "\nSCANFTK scanf"); break;
		case 16:printf( "\nPRINTFTK printf"); break;
		case 17:printf( "\nRETURNTK return"); break;
		case 18:printf("\nPLUS +"); break;
		case 19:printf("\nMINU -"); break;
		case 20:printf("\nMULT *"); break;
		case 21:printf("\nDIV /"); break;
		case 22:printf("\nLSS <"); break;
		case 23:printf("\nLEQ <="); break;
		case 24:printf("\nGRE >"); break;
		case 25:printf("\nGEQ >="); break;
		case 26:printf("\nEQL =="); break;
		case 27:printf("\nNEQ !="); break;
		case 28:printf("\nASSIGN ="); break;
		case 29:printf("\nSEMICN ;"); break;
		case 30:printf("\nCOMMA ,"); break;
		case 31:printf("\nLPARENT ("); break;
		case 32:printf("\nRPARENT )"); break;
		case 33:printf("\nLBRACK ["); break;
		case 34:printf("\nRBRACK ]"); break;
		case 35:printf("\nLBRACE {"); break;
		case 36:printf("\nRBRACE }"); break;
		}
		if(sym!=0)ch = fgetc(fp_in);
	}
	fclose(fp_in);
	fclose(fp_out);
	return 0;

}