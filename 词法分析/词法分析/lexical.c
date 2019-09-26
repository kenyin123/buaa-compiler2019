#pragma warning(disable:4996)
#include<stdio.h>
//#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"lexical.h"

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
		while (ch == 32 || ch == 33 ||( ch >= 35 && ch <= 126)) {
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
	else error(0);
	//printf("%d\n", (int)symbol);
	return 0;
}
int main() {
	fp_in = fopen("testfile.txt", "r");
	fp_out = fopen("output.txt", "w");
	ch = fgetc(fp_in);
	getsym();
	sym = (int)symbol;
	switch (sym) {
	case 1:fprintf(fp_out, "IDENFR %s", token); break;
	case 2:fprintf(fp_out, "INTCON %d", int_get); break;
	case 3:fprintf(fp_out, "CHARCON %c", char_get); break;
	case 4:fprintf(fp_out, "STRCON %s", string_get); break;
	case 5:fprintf(fp_out, "CONSTTK const"); break;
	case 6:fprintf(fp_out, "INTTK int"); break;
	case 7:fprintf(fp_out, "CHARTK char"); break;
	case 8:fprintf(fp_out, "VOIDTK void"); break;
	case 9:fprintf(fp_out, "MAINTK main"); break;
	case 10:fprintf(fp_out, "IFTK if"); break;
	case 11:fprintf(fp_out, "ELSETK else"); break;
	case 12:fprintf(fp_out, "DOTK do"); break;
	case 13:fprintf(fp_out, "WHILETK while"); break;
	case 14:fprintf(fp_out, "FORTK for"); break;
	case 15:fprintf(fp_out, "SCANFTK scanf"); break;
	case 16:fprintf(fp_out, "PRINTFTK printf"); break;
	case 17:fprintf(fp_out, "RETURNTK return"); break;
	case 18:fprintf(fp_out, "PLUS +"); break;
	case 19:fprintf(fp_out, "MINU -"); break;
	case 20:fprintf(fp_out, "MULT *"); break;
	case 21:fprintf(fp_out, "DIV /"); break;
	case 22:fprintf(fp_out, "LSS <"); break;
	case 23:fprintf(fp_out, "LEQ <="); break;
	case 24:fprintf(fp_out, "GRE >"); break;
	case 25:fprintf(fp_out, "GEQ >="); break;
	case 26:fprintf(fp_out, "EQL =="); break;
	case 27:fprintf(fp_out, "NEQ !="); break;
	case 28:fprintf(fp_out, "ASSIGN ="); break;
	case 29:fprintf(fp_out, "SEMICN ;"); break;
	case 30:fprintf(fp_out, "COMMA ,"); break;
	case 31:fprintf(fp_out, "LPARENT ("); break;
	case 32:fprintf(fp_out, "RPARENT )"); break;
	case 33:fprintf(fp_out, "LBRACK ["); break;
	case 34:fprintf(fp_out, "RBRACK ]"); break;
	case 35:fprintf(fp_out, "LBRACE {"); break;
	case 36:fprintf(fp_out, "RBRACE }"); break;
	}
	ch = fgetc(fp_in);
	while (ch != EOF) {
		getsym();
		sym = (int)symbol;
		//printf("line %d ", num_line);
		//printf("%d ", sym);
		switch (sym) {
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
		}
		ch = fgetc(fp_in);
	}
	fclose(fp_in);
	fclose(fp_out);
	system("pause");
	return 0;

}