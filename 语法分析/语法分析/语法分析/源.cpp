#pragma warning(disable:4996)
#include<stdio.h>
#include<conio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"标头.h"


//初始状态：读到了第一个字符
//结束状态：读到最后一个字符（只有一个字符）
//如果读取到无符号整数返回1，没读取到返回0
int Grammar_unsigned_int() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<无符号整数>\n");
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
			return 1;
		}
		else error(0);
	}
	else if (Grammar_unsigned_int()) {
		fprintf(fp_out, "<整数>\n");
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
		if(!Grammar_int())error(0);
		getsym();
		while (symbol == COMMA) {
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol != ASSIGN)error(0);
			getsym();
			if (!Grammar_int())error(0);
			getsym();
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<常量定义>\n");
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
			if (symbol != IDENFR)error(0);
			getsym();
			if (symbol != ASSIGN)error(0);
			getsym();
			if (symbol != CHARCON)error(0);
			getsym();
		}
		if (symbol != SEMICN)error(0);
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
	if(!Const_Definition())error(0);
	if (symbol != SEMICN)error(0);
	getsym();
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	fprintf(fp_out, "<常量说明>\n");
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
int Var_Declaration(){
	int flag; 
	int temp = Var_Definition();
	if (temp == 1)flag = 1;
	while (temp == 1) {
		temp = Var_Definition();
	}
	if (flag == 1) {
		fprintf(fp_out, "<变量说明>\n");
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
		return 1;
	}
	else if (symbol == LPARENT) {
		getsym();
		if (Expression() == 0)error(0);//读取了Expression()的第一个字符,结束时候读取了下一个字符
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<因子>\n");
		return 2;
	}
	else if (symbol == CHARCON) {
		getsym();
		fprintf(fp_out, "<因子>\n");
		return 3;
	}
	else if (symbol == IDENFR) {
		getsym();
		if ((symbol != LBRACK)&&(symbol != LPARENT)) {
			fprintf(fp_out, "<因子>\n");
			return 4;
		}
		else if(symbol == LBRACK) {
			getsym();
			if (Expression() == 0)error(0);
			if (symbol != RBRACK)error(0);
			getsym();
			fprintf(fp_out, "<因子>\n");
			return 5;
		}
		else if (symbol == LPARENT) {
			value_param_list();
			if (symbol == RPARENT) {
				getsym();
				fprintf(fp_out, "<因子>\n");
				return 6;
			}
			else error(0);
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
}
void statement_list() {
	while (symbol != RBRACE) {
		statementHandler();
	}
	fprintf(fp_out, "<语句列>\n");
}
/*
复合语句
起始结束状态默认，不含{}
*/
void complex_statement() {
	getsym();
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		if (Var_Declaration() != 0)error(0);
	}
	statement_list();
	fprintf(fp_out, "<复合语句>\n");
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
}
void if_Handler() {
	getsym();
	if (symbol != LPARENT) error(0);
	getsym();
	condition();
	if (symbol != RPARENT)error(0);
	statementHandler();
	if (symbol != ELSETK) {
		fprintf(fp_out, "<条件语句>\n");
	}
	else {
		getsym();
		statementHandler();
		fprintf(fp_out, "<条件语句>\n");
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
}
int step_length() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<步长>\n");
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
	if (symbol != LPARENT)error(0);
	getsym();
	statementHandler();
	fprintf(fp_out, "<循环语句>\n");
}

/*
值参数表
开始状态：预读第一位
结束状态：预读下一位
*/
void value_param_list() {
	if (Expression()) {
		while (symbol == SEMICN) {
			getsym();
			Expression();
		}
		fprintf(fp_out, "<值参数表>\n");
	}
	else if (symbol == RPARENT) {
		getsym();
		fprintf(fp_out, "<值参数表>\n");
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
	getsym();
}
void assign_Handler() {
	if (symbol == ASSIGN) {
		getsym();
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
	}
	else if (symbol == LBRACK) {
		if (Expression() == 0)error(0);
		if (symbol != RBRACK)error(0);
		getsym();
		if (symbol != ASSIGN)error(0);
		getsym();
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
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
}
void printf_Handler() {
	getsym();
	if (symbol != LPARENT)error(0);
	getsym();
	if (symbol == STRCON) {
		fprintf(fp_out, "<字符串>\n");
		getsym();
		if (symbol == COMMA) {
			getsym();
			Expression();
		}
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<写语句>\n");
	}
	else if (Expression()) {
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<写语句>\n");
	}
	else error(0);
}
void return_Handler() {
	getsym();
	if (symbol == SEMICN) {
		fprintf(fp_out, "<返回语句>\n");
		return;
	}
	else if (symbol == LPARENT) {
		getsym();
		if (Expression() == 0)error(0);
		if (symbol != RPARENT)error(0);
		getsym();
		fprintf(fp_out, "<写语句>\n");
	}
	else error(0);
}
void returned_func_definition() {
	getsym();
	if (symbol != IDENFR)error(0);
	fprintf(fp_out, "<声明头部>\n");
	getsym();
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym();
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<有返回值函数定义>\n");
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
			paramHandler();
			if (symbol != LBRACE)error(0);
			getsym();
			complex_statement();
			if (symbol != RBRACE)error(0);
			fprintf(fp_out, "<有返回值函数定义>\n");
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