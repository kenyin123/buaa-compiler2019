#pragma warning(disable:4996)
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"词法分析.h"

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
		while (ch == 32 || ch == 33 || ch >= 35 && ch <= 126) {
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
int main() {
	fp = fopen("1.c", "r");
	ch = fgetc(fp);
	while (ch != EOF) {
		getsym();
		sym = (int)symbol;
		//printf("line %d ", num_line);
		//printf("%d ", sym);
		switch (sym) {
		case 1:printf("IDENFR %s\n",token); break;
		case 2:printf("ITCON %d\n",int_get); break;
		case 3:printf("CHARCON %c\n",char_get); break;
		case 4:printf("STRCON %s\n",string_get); break;
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
		case 22:printf("LLS <\n"); break;
		case 23:printf("LEQ <=\n"); break;
		case 24:printf("GRE >\n"); break;
		case 25:printf("GEQ >=\n"); break;
		case 26:printf("EQL ==\n"); break;
		case 27:printf("NEQ !=\n"); break;
		case 28:printf("ASSIGN =\n"); break;
		case 29:printf("SEMICN ;\n"); break;
		case 30:printf("COMMA .\n"); break;
		case 31:printf("LPARENT (\n"); break;
		case 32:printf("RPARENT )\n"); break;
		case 33:printf("LBRACK [\n"); break;
		case 34:printf("RBRACK ]\n"); break;
		case 35:printf("LBRACE {\n"); break;
		case 36:printf("RBRACE }\n"); break;

		case 40:break;
		case 41:break;
		case 42:break;
		}
		ch = fgetc(fp);
	}

	system("pause");
	return 0;

}