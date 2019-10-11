#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

//#define idlen 64
//#define tab_size 512

FILE * fp_in;
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
/*

struct TAB {
	char idenfy[idlen];
	
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
*/



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

//初始状态：读到了第一个字符
//结束状态：读到最后一个字符（只有一个字符）
//如果读取到无符号整数返回1，没读取到返回0
int Grammar_unsigned_int() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<无符号整数>\n");
		printf("<无符号整数>\n");
		return 1;
	}
	else return 0;
}
//起始状态：读到第一个字符
//结束状态：读到最后一个字符 
//有整数返回1，没有整数return 0 并没有动作
int Grammar_int() {
	if (symbol == PLUS) {
		getsym();
		if (Grammar_unsigned_int()) {
			fprintf(fp_out, "<整数>\n");
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (symbol == MINU) {
		getsym();
		if (Grammar_unsigned_int()) {
			fprintf(fp_out, "<整数>\n");
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (Grammar_unsigned_int()) {
		fprintf(fp_out, "<整数>\n");
		printf("<整数>\n");
		return 1;
	}
	else return 0;
}

//起始状态：读取到第一个字符
//结束状态：读到";"
//有常量定义的话返回1.没有返回0
int Const_Definition() {
	if (symbol == INTTK) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();
		if (symbol != ASSIGN)error(0);
		getsym();
		if (!Grammar_int())error(0);
		getsym();
		while (symbol == COMMA) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol != ASSIGN)error(0);
			getsym();
			if (!Grammar_int())error(0);
			getsym();
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<常量定义>\n");
		printf("<常量定义>\n");
		return 1;
	}
	else if (symbol == CHARTK) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();
		if (symbol != ASSIGN)error(0);
		getsym();
		if (symbol != CHARCON)error(0);
		getsym();
		while (symbol == COMMA) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol != ASSIGN)error(0);
			getsym();
			if (symbol != CHARCON)error(0);
			getsym();
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<常量定义>\n");
		printf("<常量定义>\n");
		return 1;
	}
	else return 0;
}



//起始状态：已经识别到const！！
//结束状态：读取到；后面一个字符
//返回0 表示结束。只有一种返回值。
int Const_Declaration() {
	getsym();
	if (!Const_Definition())error(0);
	if (symbol != SEMICN)error(0);
	getsym();
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	fprintf(fp_out, "<常量说明>\n");
	printf("<常量说明>\n");
	return 0;
}

/*
起始状态：读取了第一个字符并未识别
结束状态：
return 0:没有变量定义
return 1：读到";"后一位
return 2:读到char a(
return 3:读到void
*/
int Var_Definition() {
	if (symbol == VOIDTK)return 3;
	if (symbol == CHARTK) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();

		//char a;
		//char a[2];
		//char a,b[2];
		//char a()
		if (symbol == LPARENT)return 2;//有返回函数定义
		if (symbol == LBRACK) {
			getsym();
			if (!Grammar_unsigned_int())error(0);//这里需要大于0
			if (int_get <= 0)error(0);
			getsym();
			if (symbol != RBRACK)error(0);
			getsym();
		}
		while (symbol == COMMA) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol == LBRACK) {
				getsym();
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				getsym();
				if (symbol != RBRACK)error(0);
				getsym();
			}
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<变量定义>\n");
		printf("<变量定义>\n");
	}
	else if (symbol == INTTK) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();

		//char a;
		//char a[2];
		//char a()
		//char a,b[2];
		if (symbol == LPARENT)return 2;//有返回函数定义
		if (symbol == LBRACK) {
			getsym();
			if (!Grammar_unsigned_int())error(0);//这里需要大于0
			if (int_get <= 0)error(0);
			getsym();
			if (symbol != RBRACK)error(0);
			getsym();
		}
		while (symbol == COMMA) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol == LBRACK) {
				getsym();
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				getsym();
				if (symbol != RBRACK)error(0);
				getsym();
			}
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<变量定义>\n");
		printf("<变量定义>\n");
	}
	else return 0;
	getsym();
	return 1;
}
/*
pre_flag是前面已经有变量定义，所以必须输出变量说明
结束状态：
return2：函数
return3：void
*/
int Var_Declaration() {
	int flag = 0;
	int temp = Var_Definition();
	if (temp == 1)flag = 1;
	while (temp == 1) {
		temp = Var_Definition();
	}
	if (flag == 1) {
		fprintf(fp_out, "<变量说明>\n");
		printf("<变量说明>\n");
	}
	if (temp == 0)return 0;
	if (temp == 2)return 2;
	if (temp == 3)return 3;
}
//todo 应该能合并这里以及后面，合并成一个有能力处理的函数
/*
起始状态：读取了第一个字符。
判断这里是因子的话，就应该是因子，不会有别的可能
return 0:没识别到
return 1:整数
return 2:表达式
return 3:字符
return 4:标识符
return 5:标识符[]
return 6:有返回值函数调用语句
结束状态：识别完毕并读取了接下来第一个字符
如果return 6，已经有读取！！注意？
注意顺序不能乱
最麻烦的是，如果不对，那么会不会有其他可能性
*/
int Factor() {
	if (Grammar_int() == 1) {
		getsym();
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		return 1;
	}
	else if (symbol == LPARENT) {
		getsym();
		if (Expression() == 0)error(0);//读取了Expression()的第一个字符,结束时候读取了下一个字符
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		return 2;
	}
	else if (symbol == CHARCON) {
		getsym();
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		return 3;
	}
	else if (symbol == IDENFR) {
		getsym();
		if (symbol == LBRACK) {
			getsym();
			if (Expression() == 0)error(0);
			if (symbol != RBRACK)error(0);
			getsym();
			fprintf(fp_out, "<因子>\n");
			printf("<因子>\n");
			return 5;
		}
		else if (symbol == LPARENT) {
			getsym();
			value_param_list();
			if (symbol == RPARENT) {
				getsym();
				fprintf(fp_out, "<因子>\n");
				printf("<因子>\n");
				return 6;
			}
			else error(0);
		}
		else {
			fprintf(fp_out, "<因子>\n");
			printf("<因子>\n");
			return 4;
		}
	}
	else return 0;
}
int Term() {
	if (Factor() == 0)return 0;
	while ((symbol == MULT) || (symbol == DIV)) {
		getsym();
		Factor();
	}
	fprintf(fp_out, "<项>\n");
	printf("<项>\n");
	return 1;
}
//预读下一位
int Expression() {
	if ((symbol == PLUS) || (symbol == MINU)) {
		getsym();
		if (Term()) {
			while ((symbol == PLUS) || (symbol == MINU)) {
				getsym();
				if (Term() == 0)error(0);
			}
			fprintf(fp_out, "<表达式>\n");
			printf("<表达式>\n");
			return 1;
		}
		else error(0);
	}
	else if (Term()) {
		while ((symbol == PLUS) || (symbol == MINU)) {
			getsym();
			if (Term() == 0)error(0);
		}
		fprintf(fp_out, "<表达式>\n");
		printf("<表达式>\n");
		return 1;
	}
	else return 0;
}

/*
形参表 symbol = LPARENT
*/
void paramHandler() {
	getsym();
	while (symbol != RPARENT) {
		if (symbol == INTTK) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol == COMMA)getsym();
			else if (symbol != RPARENT)error(0);
		}
		else if (symbol == CHARTK) {
			getsym();
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol == COMMA)getsym();
			else if (symbol != RPARENT)error(0);
		}
		else error(0);
	}
	fprintf(fp_out, "<参数表>\n");
	printf("<参数表>\n");
	getsym();
}
/*
语句
起始状态：预读了第一个字符
结束状态：预读下一个字符
特别注意;
*/
void statementHandler() {
	if (symbol == LBRACE) {
		getsym();
		statement_list();
		getsym();
	}
	else if (symbol == IFTK) if_Handler();
	else if (symbol == WHILETK)while_Handler();
	else if (symbol == DOTK)do_while_Handler();
	else if (symbol == FORTK)for_Handler();
	else if (symbol == SCANFTK) {
		scanf_Handler();
		if (symbol != SEMICN)error(0);
		getsym();
	}
	else if (symbol == PRINTFTK) {
		printf_Handler();
		if (symbol != SEMICN)error(0);
		getsym();
	}
	else if (symbol == RETURNTK) {
		return_Handler();
		if (symbol != SEMICN)error(0);
		getsym();
	}
	else if (symbol == SEMICN) {
		getsym();
	}
	else if (symbol == IDENFR) {
		getsym();
		if (symbol == LPARENT) {
			call_fun_Handler();
			if (symbol == SEMICN) getsym();
			else error(0);
		}
		else {
			assign_Handler();
			if (symbol == SEMICN) getsym();
			else error(0);
		}
	}
	else error(0);
	fprintf(fp_out, "<语句>\n");
	printf("<语句>\n");
}
void statement_list() {
	while (symbol != RBRACE) {
		statementHandler();
	}
	fprintf(fp_out, "<语句列>\n");
	printf("<语句列>\n");
}
/*
复合语句
起始结束状态默认，不含{}
*/
void complex_statement() {
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		if (Var_Declaration() != 0)error(0);
	}
	statement_list();
	fprintf(fp_out, "<复合语句>\n");
	printf("<复合语句>\n");
}
/*
条件
起始状态：预读第一位
结束状态：预读下一位
*/
void condition() {
	if (Expression() == 0)error(0);
	if (isRelation(symbol)) {
		getsym();
		if (Expression() == 0)error(0);
	}
	fprintf(fp_out, "<条件>\n");
	printf("<条件>\n");
}
void if_Handler() {
	getsym();
	if (symbol != LPARENT) error(0);
	getsym();
	condition();
	if (symbol != RPARENT)error(0);
	getsym();
	statementHandler();
	if (symbol != ELSETK) {
		fprintf(fp_out, "<条件语句>\n");
		printf("<条件语句>\n");
	}
	else {
		getsym();
		statementHandler();
		fprintf(fp_out, "<条件语句>\n");
		printf("<条件语句>\n");
	}
}

/*
循环语句
开始状态：
结束状态：预读下一位
*/
void while_Handler() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	condition();
	if (symbol != RPARENT)error(0);
	getsym();
	statementHandler();
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
void do_while_Handler() {
	getsym();
	statementHandler();
	if (symbol != WHILETK)error(0);
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	condition();
	if (symbol != RPARENT)error(0);
	getsym();
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
int step_length() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<步长>\n");
		printf("<步长>\n");
		return 1;
	}
	else return 0;
}
void for_Handler() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	if (symbol != IDENFR)error(0);
	getsym();
	if (symbol != ASSIGN)error(0);
	getsym();
	if (Expression() == 0)error(0);
	if (symbol != SEMICN)error(0);
	getsym();
	condition();
	if (symbol != SEMICN)error(0);
	getsym();
	if (symbol != IDENFR)error(0);
	getsym();
	if (symbol != ASSIGN)error(0);
	getsym();
	if (symbol != IDENFR)error(0);
	getsym();
	if ((symbol != PLUS) && (symbol != MINU))error(0);
	getsym();
	if (!step_length())error(0);
	getsym();
	if (symbol != RPARENT)error(0);
	getsym();
	statementHandler();
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}

/*
值参数表
开始状态：预读第一位
结束状态：预读下一位
*/
void value_param_list() {
	if (Expression()) {
		while (symbol == COMMA) {
			getsym();
			Expression();
		}
		fprintf(fp_out, "<值参数表>\n");
		printf("<值参数表>\n");
	}
	else if (symbol == RPARENT) {
		fprintf(fp_out, "<值参数表>\n");
		printf("<值参数表>\n");
	}
	else error(0);
}

/*
函数调用
起始状态：(
*/
void call_fun_Handler() {
	getsym();
	value_param_list();
	if (symbol != RPARENT)error(0);
	fprintf(fp_out, "<函数调用语句>\n");
	printf("<函数调用语句>\n");
	getsym();
}
void assign_Handler() {
	if (symbol == ASSIGN) {
		getsym();
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
		printf("<赋值语句>\n");
	}
	else if (symbol == LBRACK) {
		getsym();
		if (Expression() == 0)error(0);
		if (symbol != RBRACK)error(0);
		getsym();
		if (symbol != ASSIGN)error(0);
		getsym();
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
		printf("<赋值语句>\n");
	}
	else error(0);
}
void scanf_Handler() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	if (symbol != IDENFR)error(0);
	getsym();
	while (symbol == COMMA) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();
	}
	if (symbol != RPARENT)error(0);
	getsym();
	fprintf(fp_out, "<读语句>\n");
	printf("<读语句>\n");
}
void printf_Handler() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	if (symbol == STRCON) {
		fprintf(fp_out, "<字符串>\n");
		printf("<字符串>\n");
		getsym();
		if (symbol == COMMA) {
			getsym();
			Expression();
		}
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<写语句>\n");
		printf("<写语句>\n");
	}
	else if (Expression()) {
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<写语句>\n");
		printf("<写语句>\n");
	}
	else error(0);
}
void return_Handler() {
	getsym();
	if (symbol == SEMICN) {
		fprintf(fp_out, "<返回语句>\n");
		printf("<返回语句>\n");
		return;
	}
	else if (symbol == LPARENT) {
		getsym();
		if (Expression() == 0)error(0);
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<返回语句>\n");
		printf("<返回语句>\n");
	}
	else error(0);
}
void returned_func_definition() {
	getsym();
	if (symbol != IDENFR)error(0);
	fprintf(fp_out, "<声明头部>\n");
	printf("<声明头部>\n");
	getsym();
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym();
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<有返回值函数定义>\n");
	printf("<有返回值函数定义>\n");
	getsym();
}
int unreturn_func_definition() {
	getsym();
	if (symbol == MAINTK)return 1;
	if (symbol != IDENFR)error(0);
	getsym();
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym();
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<无返回值函数定义>\n");
	printf("<无返回值函数定义>\n");
	getsym();
	return 0;
}
void mainfunc() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	if (symbol != RPARENT)error(0);
	getsym();
	if (symbol != LBRACE)error(0);
	getsym();
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<主函数>\n");
	printf("<主函数>\n");
}
void program() {
	getsym();
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		int temp = Var_Declaration();
		if (temp == 2) {
			fprintf(fp_out, "<声明头部>\n");
			printf("<声明头部>\n");
			paramHandler();
			if (symbol != LBRACE)error(0);
			getsym();
			complex_statement();
			if (symbol != RBRACE)error(0);
			fprintf(fp_out, "<有返回值函数定义>\n");
			printf("<有返回值函数定义>\n");
			getsym();
		}
	}
	while (symbol == VOIDTK || symbol == INTTK || symbol == CHARTK) {
		if (symbol == VOIDTK) {
			if (unreturn_func_definition() == 1) {
				break;
			}
		}
		else if (symbol == INTTK || symbol == CHARTK) {
			returned_func_definition();
		}
		else error(0);
	}
	mainfunc();
	fprintf(fp_out, "<程序>\n");
	printf("<程序>\n");
	getsym();
	if (symbol != UNKNOWN)error(0);
}
int main() {
	fp_in = fopen("testfile.txt", "r");
	fp_out = fopen("output.txt", "w");
	program();
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}