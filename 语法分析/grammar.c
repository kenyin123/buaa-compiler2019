#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"grammar.h"

/*
初始状态：读到了第一个字符
结束状态：读到最后一个字符（只有一个字符）
如果读取到无符号整数返回1，没读取到返回0
无预读
*/
int Grammar_unsigned_int() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<无符号整数>\n");
		printf("<无符号整数>\n");
		return 1;
	}
	else return 0;
}

/*
起始状态：读到第一个字符
结束状态：读到最后一个字符 
有整数返回1，没有整数return 0
无预读
*/
int Grammar_int() {
	if (symbol == PLUS) {
		getsym(0);
		if (Grammar_unsigned_int()) {
			fprintf(fp_out, "<整数>\n");
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (symbol == MINU) {
		getsym(0);
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

/*
起始状态：读取到第一个字符
结束状态：读到";"
有常量定义的话返回1.没有返回0
有预读;已经解决
*/
int Const_Definition() {
	//int pre_read[100];
	//int pre_len = 0;
	if (symbol == INTTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (!Grammar_int())error(0);
		getsym(1);
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(0);
			if (symbol != ASSIGN)error(0);
			getsym(0);
			if (!Grammar_int())error(0);
			getsym(1);
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<常量定义>\n");
		printf("<常量定义>\n");
		print_symbol(symbol);
		return 1;
	}
	else if (symbol == CHARTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (symbol != CHARCON)error(0);
		getsym(1);
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(0);
			if (symbol != ASSIGN)error(0);
			getsym(0);
			if (symbol != CHARCON)error(0);
			getsym(1);
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<常量定义>\n");
		printf("<常量定义>\n");
		print_symbol(symbol);
		return 1;
	}
	else return 0;
}

/*
起始状态：已经识别到const并输出
结束状态：预读一个字符并输出
返回0 表示结束。只有一种返回值。
*/
int Const_Declaration() {
	getsym(0);
	if (!Const_Definition())error(0);
	if (symbol != SEMICN)error(0);
	getsym(1);
	while (symbol == CONSTTK) {
		print_symbol(symbol);
		getsym(0);
		if (!Const_Definition())error(0);
		if (symbol != SEMICN)error(0);
		getsym(1);
	}
	fprintf(fp_out, "<常量说明>\n");
	printf("<常量说明>\n");
	print_symbol(symbol);
	return 0;
}

/*
起始状态：读取了第一个字符并未识别
结束状态：
return 0:没有变量定义
return 1：读到";"并全部输出
return 2:读到char a(并全部输出
return 3:读到void并全部输出
char a;
char a[2];
char a,b[2];
char a()
*/
int Var_Definition() {
	if (symbol == VOIDTK)return 3;
	if (symbol == CHARTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		getsym(1);
		if (symbol == LPARENT) {
			print_symbol(symbol);
			return 2;//有返回函数定义
		}
		if (symbol == LBRACK) {
			print_symbol(symbol);
			getsym(0);
			if (!Grammar_unsigned_int())error(0);//这里需要大于0
			if (int_get <= 0)error(0);
			getsym(0);
			if (symbol != RBRACK)error(0);
			getsym(1);
		}
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(1);
			if (symbol == LBRACK) {
				print_symbol(symbol);
				getsym(0);
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				getsym(0);
				if (symbol != RBRACK)error(0);
				getsym(1);
			}
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<变量定义>\n");
		printf("<变量定义>\n");
		print_symbol(symbol);
	}
	else if (symbol == INTTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		getsym(1);
		if (symbol == LPARENT) {
			print_symbol(symbol);
			return 2;//有返回函数定义
		}
		if (symbol == LBRACK) {
			print_symbol(symbol);
			getsym(0);
			if (!Grammar_unsigned_int())error(0);//这里需要大于0
			if (int_get <= 0)error(0);
			getsym(0);
			if (symbol != RBRACK)error(0);
			getsym(1);
		}
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(1);
			if (symbol == LBRACK) {
				print_symbol(symbol);
				getsym(0);
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				getsym(0);
				if (symbol != RBRACK)error(0);
				getsym(1);
			}
		}
		if (symbol != SEMICN)error(0);
		fprintf(fp_out, "<变量定义>\n");
		printf("<变量定义>\n");
		print_symbol(symbol);
	}
	else return 0;
	return 1;
}
/*
pre_flag是前面已经有变量定义，所以必须输出变量说明
结束状态：
return 0:正常结束到最后
return2：函数char a(
return3：void
注意：未区分char int
*/
int Var_Declaration() {
	sym_str* head = (sym_str*)malloc(sizeof(sym_str));
	sym_str* p = head;

	int flag  = Var_Definition();
	while (1) {
		getsym(1);
		p = enter_sym_str(p);
		if (symbol == VOIDTK) {
			if (flag == 1) {
				fprintf(fp_out, "<变量说明>\n");
				printf("<变量说明>\n");
			}
			printf_sym_str(head);
			return 3;
		}
		if (symbol == CHARTK || symbol == INTTK) {
			getsym(1);
			if (symbol != IDENFR)error(0);
			p = enter_sym_str(p);
			//状态：char a
			getsym(1);
			if (symbol == LPARENT) {
				if (flag == 1) {
					fprintf(fp_out, "<变量说明>\n");
					printf("<变量说明>\n");
				}
				printf_sym_str(head);
				p = head;
				fprintf(fp_out, "<声明头部>\n");
				printf("<声明头部>\n");
				print_symbol(symbol);
				return 2;//有返回函数定义
			}
			if (symbol == LBRACK) {
				p = enter_sym_str(p);
				getsym(1);
				p = enter_sym_str(p);
				printf_sym_str(head);
				p = head;
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				getsym(1);
				p = enter_sym_str(p);
				if (symbol != RBRACK)error(0);
				//状态：char a [100]
				getsym(1);
			}
			while (symbol == COMMA) {
				p = enter_sym_str(p);
				getsym(1);
				p = enter_sym_str(p);
				if (symbol != IDENFR)error(0);
				getsym(1);
				if (symbol == LBRACK) {
					p = enter_sym_str(p);
					getsym(1);
					p = enter_sym_str(p);
					printf_sym_str(head);
					p = head;
					if (!Grammar_unsigned_int())error(0);//这里需要大于0
					if (int_get <= 0)error(0);
					getsym(1);
					p = enter_sym_str(p);
					if (symbol != RBRACK)error(0);
					getsym(1);
				}
			}
			// 3 好像有一个没有输出
			if (symbol != SEMICN)error(0);
			printf_sym_str(head);
			p = head;
			fprintf(fp_out, "<变量定义>\n");
			printf("<变量定义>\n");
			p = enter_sym_str(p);
			printf_sym_str(head);
			p = head;
			continue;
		}
		else {
			if (flag == 1) {
				fprintf(fp_out, "<变量说明>\n");
				printf("<变量说明>\n");
				if (symbol != RBRACE) {//没有语句列的情况
					printf_sym_str(head);
				p = head;
				}
				
			}
			return 0;
		} 
	}
}

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
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 1;
	}
	else if (symbol == LPARENT) {
		getsym(0);
		if (Expression() == 0)error(0);//读取了Expression()的第一个字符,结束时候读取了下一个字符
		print_symbol(symbol);
		if (symbol != RPARENT)error(0);
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 2;
	}
	else if (symbol == CHARCON) {
		fprintf(fp_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 3;
	}
	else if (symbol == IDENFR) {
		getsym(1);
		if (symbol == LBRACK) {
			print_symbol(symbol);
			getsym(0);
			if (Expression() == 0)error(0);
			print_symbol(symbol);
			if (symbol != RBRACK)error(0);
			fprintf(fp_out, "<因子>\n");
			printf("<因子>\n");
			getsym(1);
			return 5;
		}
		else if (symbol == LPARENT) {
			print_symbol(symbol);
			call_fun_Handler();
			fprintf(fp_out, "<因子>\n");
			printf("<因子>\n");
			return 6;
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
		print_symbol(symbol);
		getsym(0);
		if (Factor() == 0)error(0);
	}
	fprintf(fp_out, "<项>\n");
	printf("<项>\n");
	return 1;
}
/*
结尾symbol没有打印
*/
int Expression() {
	if ((symbol == PLUS) || (symbol == MINU)) {
		getsym(0);
		if (Term()) {
			while ((symbol == PLUS) || (symbol == MINU)) {
				print_symbol(symbol);
				getsym(0);
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
			print_symbol(symbol);
			getsym(0);
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
读取)下一位并输出
*/
void paramHandler() {
	getsym(1);
	while (symbol != RPARENT) {
		print_symbol(symbol);
		if (symbol == INTTK) {
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(1);
			if (symbol == COMMA) {
				print_symbol(symbol);
				getsym(1);
			}
			else if (symbol != RPARENT)error(0);
		}
		else if (symbol == CHARTK) {
			getsym(0);
			if (symbol != IDENFR)error(0);
			getsym(1);
			if (symbol == COMMA) {
				print_symbol(symbol);
				getsym(1);
			}
			else if (symbol != RPARENT)error(0);
		}
		else error(0);
	}
	fprintf(fp_out, "<参数表>\n");
	printf("<参数表>\n");
	print_symbol(symbol);
	getsym(0);
}
/*
语句
起始状态：预读了第一个字符
结束状态：预读下一个字符并不输出
*/
void statementHandler() {
	if (symbol == LBRACE) {
		getsym(0);
		statement_list();
		print_symbol(symbol);
		getsym(1);
	}
	else if (symbol == IFTK) if_Handler();
	else if (symbol == WHILETK)while_Handler();
	else if (symbol == DOTK)do_while_Handler();
	else if (symbol == FORTK)for_Handler();
	else if (symbol == SCANFTK) {
		scanf_Handler();
		if (symbol != SEMICN)error(0);
		getsym(1);
	}
	else if (symbol == PRINTFTK) {
		printf_Handler();
		if (symbol != SEMICN)error(0);
		getsym(1);
	}
	else if (symbol == RETURNTK) {
		return_Handler();
		if (symbol != SEMICN)error(0);
		getsym(1);
	}
	else if (symbol == SEMICN) {
		getsym(1);
	}
	else if (symbol == IDENFR) {
		getsym(0);
		if (symbol == LPARENT) {
			call_fun_Handler();
			print_symbol(symbol);
			if (symbol == SEMICN) getsym(1);
			else error(0);
		}
		else {
			assign_Handler();
			if (symbol == SEMICN) getsym(1);
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
		if (symbol != RBRACE)print_symbol(symbol);
	}
	fprintf(fp_out, "<语句列>\n");
	printf("<语句列>\n");
}
/*
复合语句
起始状态读取第一个并没有输出
读取}并输出
*/
void complex_statement() {
	if (symbol != RBRACE)print_symbol(symbol);
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		if (Var_Declaration() != 0)error(0);
	}
	statement_list();
	fprintf(fp_out, "<复合语句>\n");
	printf("<复合语句>\n");
	print_symbol(symbol);
}
/*
条件
起始状态：预读第一位
结束状态：预读下一位并输出
*/
void condition() {
	if (Expression() == 0)error(0);
	if (isRelation(symbol)) {
		print_symbol(symbol);
		getsym(0);
		if (Expression() == 0)error(0);
	}
	fprintf(fp_out, "<条件>\n");
	printf("<条件>\n");
	print_symbol(symbol);
}
void if_Handler() {
	getsym(0);
	if (symbol != LPARENT) error(0);
	getsym(0);
	condition();
	if (symbol != RPARENT)error(0);
	getsym(0);
	statementHandler();
	if (symbol != ELSETK) {
		fprintf(fp_out, "<条件语句>\n");
		printf("<条件语句>\n");
	}
	else {
		print_symbol(symbol);
		getsym(0);
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
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	condition();
	if (symbol != RPARENT)error(0);
	getsym(0);
	statementHandler();
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
void do_while_Handler() {
	getsym(0);
	statementHandler();
	if (symbol != WHILETK)error(0);
	print_symbol(symbol);
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	condition();
	if (symbol != RPARENT)error(0);
	getsym(1);
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
int step_length() {
	if (symbol == INTCON) {
		fprintf(fp_out, "<无符号整数>\n");
		printf("<无符号整数>\n");
		fprintf(fp_out, "<步长>\n");
		printf("<步长>\n");
		return 1;
	}
	else return 0;
}
void for_Handler() {
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol != IDENFR)error(0);
	getsym(0);
	if (symbol != ASSIGN)error(0);
	getsym(0);
	if (Expression() == 0)error(0);
	print_symbol(symbol);
	if (symbol != SEMICN)error(0);
	getsym(0);
	condition();
	if (symbol != SEMICN)error(0);
	getsym(0); 
	if (symbol != IDENFR)error(0);
	getsym(0);
	if (symbol != ASSIGN)error(0);
	getsym(0);
	if (symbol != IDENFR)error(0);
	getsym(0);
	if ((symbol != PLUS) && (symbol != MINU))error(0);
	getsym(0);
	if (!step_length())error(0);
	getsym(0);
	if (symbol != RPARENT)error(0);
	getsym(0);
	statementHandler();
	fprintf(fp_out, "<循环语句>\n");
	printf("<循环语句>\n");
}

/*
值参数表
开始状态：预读第一位并没有输出
结束状态：预读下一位)并输出
*/
void value_param_list() {
	if (symbol == RPARENT) {
		fprintf(fp_out, "<值参数表>\n");
		printf("<值参数表>\n");
		print_symbol(symbol);
		return;
	}
	print_symbol(symbol);
	if (Expression()) {
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			Expression();

		}
		if (symbol != RPARENT)error(0);
		fprintf(fp_out, "<值参数表>\n");
		printf("<值参数表>\n");
		print_symbol(symbol);
	}
	else error(0);
}

/*
函数调用
起始状态：(
*/
void call_fun_Handler() {
	getsym(1);
	value_param_list();
	fprintf(fp_out, "<有返回值函数调用语句>\n");
	printf("<有返回值函数调用语句>\n");
	getsym(1);
}
void assign_Handler() {
	if (symbol == ASSIGN) {
		getsym(0);
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
		printf("<赋值语句>\n");
		print_symbol(symbol);
	}
	else if (symbol == LBRACK) {
		getsym(0);
		if (Expression() == 0)error(0);
		print_symbol(symbol);
		if (symbol != RBRACK)error(0);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (Expression() == 0)error(0);
		fprintf(fp_out, "<赋值语句>\n");
		printf("<赋值语句>\n");
		print_symbol(symbol);
	}
	else error(0);
}
void scanf_Handler() {
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol != IDENFR)error(0);
	getsym(0);
	while (symbol == COMMA) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		getsym(0);
	}
	if (symbol != RPARENT)error(0);
	getsym(1);
	fprintf(fp_out, "<读语句>\n");
	printf("<读语句>\n");
	print_symbol(symbol);
}
void printf_Handler() {
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol == STRCON) {
		fprintf(fp_out, "<字符串>\n");
		printf("<字符串>\n");
		getsym(0);
		if (symbol == COMMA) {
			getsym(0);
			Expression();
			print_symbol(symbol);
		}
		if (symbol != RPARENT)error(0);
		getsym(1);
		fprintf(fp_out, "<写语句>\n");
		printf("<写语句>\n");
		print_symbol(symbol);
	}
	else if (Expression()) {
		print_symbol(symbol);
		if (symbol != RPARENT)error(0);
		getsym(1);
		fprintf(fp_out, "<写语句>\n");
		printf("<写语句>\n");
		print_symbol(symbol);
	}
	else error(0);
}
void return_Handler() {
	getsym(1);
	if (symbol == SEMICN) {
		fprintf(fp_out, "<返回语句>\n");
		printf("<返回语句>\n");
		print_symbol(symbol);
		return;
	}
	else if (symbol == LPARENT) {
		print_symbol(symbol);
		getsym(0);
		if (Expression() == 0)error(0);
		print_symbol(symbol);
		if (symbol != RPARENT)error(0);
		getsym(1);
		fprintf(fp_out, "<返回语句>\n");
		printf("<返回语句>\n");
		print_symbol(symbol);
	}
	else error(0);
}
void returned_func_definition() {
	getsym(0);
	if (symbol != IDENFR)error(0);
	fprintf(fp_out, "<声明头部>\n");
	printf("<声明头部>\n");
	getsym(0);
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<有返回值函数定义>\n");
	printf("<有返回值函数定义>\n");
	getsym(0);
}
int unreturn_func_definition() {
	getsym(0);
	if (symbol == MAINTK)return 1;
	if (symbol != IDENFR)error(0);
	getsym(0);
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<无返回值函数定义>\n");
	printf("<无返回值函数定义>\n");
	getsym(0);
	return 0;
}
void mainfunc() {
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol != RPARENT)error(0);
	getsym(0);
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement();
	if (symbol != RBRACE)error(0);
	fprintf(fp_out, "<主函数>\n");
	printf("<主函数>\n");
}
void program() {
	getsym(0);
	//if (symbol != CONSTTK)print_symbol(symbol);
	if (symbol == CONSTTK) {
		Const_Declaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		int temp = Var_Declaration();
		if (temp == 2) {
			paramHandler();
			if (symbol != LBRACE)error(0);
			getsym(1);
			complex_statement();
			if (symbol != RBRACE)error(0);
			fprintf(fp_out, "<有返回值函数定义>\n");
			printf("<有返回值函数定义>\n");
			getsym(0);
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
	getsym(1);
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