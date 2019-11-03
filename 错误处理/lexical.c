#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"
int num_line_temp = 0;

void clear() {
	int i = 0;
	for (i = 0; i < sizeof(token); i++) {
		token[i] = '\0';
	}
}
int isNewline(char a) {
	if (a == '\n')return 1;
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
int isRelation(enum SYMBOL symbol) {
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

int getsym(int flag)
{
	if (error_klmn) {
		clear();
		error_klmn = 0;
		symbol = symbol_error_klmn;
		symbol_error_klmn = UNKNOWN;
		return 0;
	}
	num_line_temp = num_line;
	ch = fgetc(fp_in);
	symbol = UNKNOWN;
	clear();
	
	while (isspace(ch)) {
		if (isNewline(ch)) {
			num_line++;
		}
		ch = fgetc(fp_in);
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
				error(1);
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
		while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
			catToken(ch);
			ch = fgetc(fp_in);
		}
		if (ch != '\"') {
			error(1);
			return 0;
		}
		symbol = STRCON;
	}
	else if (isQuotation(ch)) {
		ch = fgetc(fp_in);
		if (isDigit(ch) || isLetter(ch) || (ch == '*') || (ch == '/') || (ch == '+') || (ch == '-'))symbol = CHARCON;
		else {
			error(1);
			return 0;
		} 
		catToken(ch);
		ch = fgetc(fp_in);
		if (ch != '\'')error(1);
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
	else {
		error(1);
		return 0;
	}
	if (flag == 0) {
		print_symbol(symbol);
	}
	return 0;
}

void print_symbol(enum SYMBOL sym) {
	switch ((int)sym) {
	case 0:printf("UNKNOWN\n"); break;
	case 1:printf("IDENFR %s\n", token); break;
	case 2:printf("INTCON %s\n", token); break;
	case 3:printf("CHARCON %s\n", token); break;
	case 4:printf("STRCON %s\n", token); break;
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
	switch ((int)symbol) {
	case 0:break;
	case 1:fprintf(grammar_out, "IDENFR %s\n", token); break;
	case 2:fprintf(grammar_out, "INTCON %s\n", token); break;
	case 3:fprintf(grammar_out, "CHARCON %s\n", token); break;
	case 4:fprintf(grammar_out, "STRCON %s\n", token); break;
	case 5:fprintf(grammar_out, "CONSTTK const\n"); break;
	case 6:fprintf(grammar_out, "INTTK int\n"); break;
	case 7:fprintf(grammar_out, "CHARTK char\n"); break;
	case 8:fprintf(grammar_out, "VOIDTK void\n"); break;
	case 9:fprintf(grammar_out, "MAINTK main\n"); break;
	case 10:fprintf(grammar_out, "IFTK if\n"); break;
	case 11:fprintf(grammar_out, "ELSETK else\n"); break;
	case 12:fprintf(grammar_out, "DOTK do\n"); break;
	case 13:fprintf(grammar_out, "WHILETK while\n"); break;
	case 14:fprintf(grammar_out, "FORTK for\n"); break;
	case 15:fprintf(grammar_out, "SCANFTK scanf\n"); break;
	case 16:fprintf(grammar_out, "PRINTFTK printf\n"); break;
	case 17:fprintf(grammar_out, "RETURNTK return\n"); break;
	case 18:fprintf(grammar_out, "PLUS +\n"); break;
	case 19:fprintf(grammar_out, "MINU -\n"); break;
	case 20:fprintf(grammar_out, "MULT *\n"); break;
	case 21:fprintf(grammar_out, "DIV /\n"); break;
	case 22:fprintf(grammar_out, "LSS <\n"); break;
	case 23:fprintf(grammar_out, "LEQ <=\n"); break;
	case 24:fprintf(grammar_out, "GRE >\n"); break;
	case 25:fprintf(grammar_out, "GEQ >=\n"); break;
	case 26:fprintf(grammar_out, "EQL ==\n"); break;
	case 27:fprintf(grammar_out, "NEQ !=\n"); break;
	case 28:fprintf(grammar_out, "ASSIGN =\n"); break;
	case 29:fprintf(grammar_out, "SEMICN ;\n"); break;
	case 30:fprintf(grammar_out, "COMMA ,\n"); break;
	case 31:fprintf(grammar_out, "LPARENT (\n"); break;
	case 32:fprintf(grammar_out, "RPARENT )\n"); break;
	case 33:fprintf(grammar_out, "LBRACK [\n"); break;
	case 34:fprintf(grammar_out, "RBRACK ]\n"); break;
	case 35:fprintf(grammar_out, "LBRACE {\n"); break;
	case 36:fprintf(grammar_out, "RBRACE }\n"); break;
	}
}

sym_str* enter_sym_str(sym_str* p) {
	p->symbol = (int)symbol;
	for (int i = 0; i < sizeof(p->token); i++) {
		(p->token)[i] = '\0';
	}
	for (int i = 0; i < strlen(token); i++) {
		(p->token)[i] = token[i];
	}
	sym_str* new_p = (sym_str*)malloc(sizeof(sym_str));
	p->next_symbol = new_p;
	new_p->next_symbol = NULL;
	return new_p;
}
void printf_sym_str(sym_str* p) {
	while (p != NULL) {
		switch ((p->symbol)) {
		case 0:printf("UNKNOWN\n"); break;
		case 1:printf("IDENFR %s\n", p->token); break;
		case 2:printf("INTCON %s\n", p->token); break;
		case 3:printf("CHARCON %s\n", p->token); break;
		case 4:printf("STRCON %s\n", p->token); break;
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
		switch (p->symbol) {
		case 0:break;
		case 1:fprintf(grammar_out, "IDENFR %s\n", p->token); break;
		case 2:fprintf(grammar_out, "INTCON %s\n", p->token); break;
		case 3:fprintf(grammar_out, "CHARCON %s\n", p->token); break;
		case 4:fprintf(grammar_out, "STRCON %s\n", p->token); break;
		case 5:fprintf(grammar_out, "CONSTTK const\n"); break;
		case 6:fprintf(grammar_out, "INTTK int\n"); break;
		case 7:fprintf(grammar_out, "CHARTK char\n"); break;
		case 8:fprintf(grammar_out, "VOIDTK void\n"); break;
		case 9:fprintf(grammar_out, "MAINTK main\n"); break;
		case 10:fprintf(grammar_out, "IFTK if\n"); break;
		case 11:fprintf(grammar_out, "ELSETK else\n"); break;
		case 12:fprintf(grammar_out, "DOTK do\n"); break;
		case 13:fprintf(grammar_out, "WHILETK while\n"); break;
		case 14:fprintf(grammar_out, "FORTK for\n"); break;
		case 15:fprintf(grammar_out, "SCANFTK scanf\n"); break;
		case 16:fprintf(grammar_out, "PRINTFTK printf\n"); break;
		case 17:fprintf(grammar_out, "RETURNTK return\n"); break;
		case 18:fprintf(grammar_out, "PLUS +\n"); break;
		case 19:fprintf(grammar_out, "MINU -\n"); break;
		case 20:fprintf(grammar_out, "MULT *\n"); break;
		case 21:fprintf(grammar_out, "DIV /\n"); break;
		case 22:fprintf(grammar_out, "LSS <\n"); break;
		case 23:fprintf(grammar_out, "LEQ <=\n"); break;
		case 24:fprintf(grammar_out, "GRE >\n"); break;
		case 25:fprintf(grammar_out, "GEQ >=\n"); break;
		case 26:fprintf(grammar_out, "EQL ==\n"); break;
		case 27:fprintf(grammar_out, "NEQ !=\n"); break;
		case 28:fprintf(grammar_out, "ASSIGN =\n"); break;
		case 29:fprintf(grammar_out, "SEMICN ;\n"); break;
		case 30:fprintf(grammar_out, "COMMA ,\n"); break;
		case 31:fprintf(grammar_out, "LPARENT (\n"); break;
		case 32:fprintf(grammar_out, "RPARENT )\n"); break;
		case 33:fprintf(grammar_out, "LBRACK [\n"); break;
		case 34:fprintf(grammar_out, "RBRACK ]\n"); break;
		case 35:fprintf(grammar_out, "LBRACE {\n"); break;
		case 36:fprintf(grammar_out, "RBRACE }\n"); break;
		}
		p = p->next_symbol;
	}
}

