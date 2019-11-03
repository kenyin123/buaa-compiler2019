#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"
FILE* fp_in;
FILE* grammar_out;
FILE* error_out;
FILE* tab_out;

char ch;//最新读取到的字符
int num_line = 1;//代码行数
char token[token_len];//截取的一段，字符串或者数字等
int int_get;//int类型数字
int num_func = 0;
//char ident[token_len];//暂时存储函数名字，用于函数调用和赋值语句
int expr_is_char = 0;//判断表达式是不是单个字符

char pro[13][20] = { "const","int","char","void","main","if","else","do","while","for","scanf","printf","return"
}; //保留字表
enum SYMBOL symbol;


/*
初始状态：读到了第一个字符
结束状态：读到最后一个字符（只有一个字符）
如果读取到无符号整数返回1，没读取到返回0
无预读
*/
int Grammar_unsigned_int() {
	if (symbol == INTCON) {
		fprintf(grammar_out, "<无符号整数>\n");
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
			fprintf(grammar_out, "<整数>\n");
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (symbol == MINU) {
		getsym(0);
		if (Grammar_unsigned_int()) {
			fprintf(grammar_out, "<整数>\n");
			printf("<整数>\n");
			int_get = 0 - int_get;
			return 1;
		}
		else error(0);
	}
	else if (Grammar_unsigned_int()) {
		fprintf(grammar_out, "<整数>\n");
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
int Const_Definition(int field_flag) {
	char token_temp[idlen] = { '\0' };//存储ident
	if (symbol == INTTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		str_cpy(token, token_temp);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (!Grammar_int())error(15);
		entertab(token_temp,const_int, int_get, addr, field_flag);//const_int登录符号表
		addr += 4;
		getsym(1);
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			str_cpy(token, token_temp);
			getsym(0);
			if (symbol != ASSIGN)error(0);
			getsym(0);
			if (!Grammar_int())error(15);
			entertab(token_temp, const_int, int_get, addr, field_flag);//const_int登录符号表
			addr += 4;
			getsym(1);
		}
		if (symbol != SEMICN)error(11);
		fprintf(grammar_out, "<常量定义>\n");
		printf("<常量定义>\n");
		print_symbol(symbol);
		return 1;
	}
	else if (symbol == CHARTK) {
		getsym(0);
		if (symbol != IDENFR)error(0);
		str_cpy(token, token_temp);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (symbol != CHARCON)error(15);
		entertab(token_temp, const_char, token[0], addr, field_flag);//const_char登录符号表
		addr += 4;
		getsym(1);
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			str_cpy(token, token_temp);
			getsym(0);
			if (symbol != ASSIGN)error(0);
			getsym(0);
			if (symbol != CHARCON)error(15);
			entertab(token_temp, const_char, token[0], addr, field_flag);//const_char登录符号表
			addr += 4;
			getsym(1);
		}
		if (symbol != SEMICN)error(11);
		fprintf(grammar_out, "<常量定义>\n");
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
int Const_Declaration(int field_flag) {
	getsym(0);
	if (!Const_Definition(field_flag))error(0);
	if (symbol != SEMICN)error(11);
	getsym(1);
	while (symbol == CONSTTK) {
		print_symbol(symbol);
		getsym(0);
		if (!Const_Definition(field_flag))error(0);
		if (symbol != SEMICN)error(11);
		getsym(1);
	}
	fprintf(grammar_out, "<常量说明>\n");
	printf("<常量说明>\n");
	print_symbol(symbol);
	return 0;
}

/*
开始状态：读了第一个字符并输出
结束状态：
return 0:正常结束到最后
return 1:int a(
return 2:char a(
return 3:void
注意：未区分char int
char a;
char a[2];
char a,b[2];
char a()
*/
int Var_Declaration(int field_flag) {
	sym_str* head = (sym_str*)malloc(sizeof(sym_str));
	sym_str* p = head;
	char token_temp[idlen] = {'\0'};
	enum SYMBOL symbol_temp;

	if (symbol == VOIDTK)return 3;
	if (symbol == CHARTK || symbol == INTTK)  {
		symbol_temp = symbol;
		getsym(0);
		if (symbol != IDENFR)error(0);
		str_cpy(token, token_temp);
		getsym(1);
		if (symbol == LPARENT) {
			entertab(token_temp, (symbol_temp == INTTK)? return_int_func: return_char_func, NULL, addr, field_flag);
			num_func++;
			fprintf(grammar_out, "<声明头部>\n");
			printf("<声明头部>\n");
			print_symbol(symbol);
			return  (symbol_temp == INTTK) ? 1:2;
		}
		if (symbol == LBRACK) {
			print_symbol(symbol);
			getsym(0);
			if (!Grammar_unsigned_int())error(0);//这里需要大于0
			if (int_get <= 0)error(0);
			entertab(token_temp, (symbol_temp == INTTK) ? int_array:char_array, int_get, addr, field_flag);
			addr += tab[tab_loc - 1].size;
			getsym(0);
			if (symbol != RBRACK)error(13);
			getsym(1);
		}
		else {
			entertab(token_temp, (symbol_temp == INTTK) ? var_int:var_char, NULL, addr, field_flag);
			addr += sizeof(int);
		}
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			if (symbol != IDENFR)error(0);
			str_cpy(token, token_temp);
			getsym(1);
			if (symbol == LBRACK) {
				print_symbol(symbol);
				getsym(0);
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				entertab(token_temp, (symbol_temp == INTTK) ? int_array : char_array, int_get, addr, field_flag);
				getsym(0);
				if (symbol != RBRACK)error(13);
				getsym(1);
			}
			else {
				entertab(token_temp, (symbol_temp == INTTK) ? var_int:var_char, NULL, addr, field_flag);
				addr += sizeof(int);
			}
		}
		if (symbol != SEMICN)error(11);
		fprintf(grammar_out, "<变量定义>\n");
		printf("<变量定义>\n");
		print_symbol(symbol);
	}
	while (1) {
		getsym(1);
		p = enter_sym_str(p);
		if (symbol == VOIDTK) {
			fprintf(grammar_out, "<变量说明>\n");
			printf("<变量说明>\n");
			printf_sym_str(head);
			p = head;
			return 3;
		}
		if (symbol == CHARTK || symbol == INTTK) {
			symbol_temp = symbol;
			getsym(1);
			if (symbol != IDENFR)error(0);
			str_cpy(token, token_temp);
			p = enter_sym_str(p);
			//状态：char a
			getsym(1);
			if (symbol == LPARENT) {
				entertab(token_temp, (symbol_temp == INTTK) ? return_int_func : return_char_func, NULL, addr, field_flag);
				num_func++;
				fprintf(grammar_out, "<变量说明>\n");
				printf("<变量说明>\n");
				printf_sym_str(head);
				p = head;
				fprintf(grammar_out, "<声明头部>\n");
				printf("<声明头部>\n");
				print_symbol(symbol);
				return  (symbol_temp == INTTK) ? 1 : 2;
			}
			if (symbol == LBRACK) {
				p = enter_sym_str(p);
				getsym(1);
				p = enter_sym_str(p);
				printf_sym_str(head);
				p = head;
				if (!Grammar_unsigned_int())error(0);//这里需要大于0
				if (int_get <= 0)error(0);
				entertab(token_temp, (symbol_temp == INTTK) ? int_array : char_array, int_get, addr, field_flag);
				addr += tab[tab_loc - 1].size;
				getsym(1);
				p = enter_sym_str(p);
				if (symbol != RBRACK)error(13);
				//状态：char a [100]
				getsym(1);
			}
			else {
				entertab(token_temp, (symbol_temp == INTTK) ? var_int : var_char, NULL, addr, field_flag);
				addr += sizeof(int);
			}
			while (symbol == COMMA) {
				p = enter_sym_str(p);
				getsym(1);
				p = enter_sym_str(p);
				if (symbol != IDENFR)error(0);
				str_cpy(token, token_temp);
				getsym(1);
				if (symbol == LBRACK) {
					p = enter_sym_str(p);
					getsym(1);
					p = enter_sym_str(p);
					printf_sym_str(head);
					p = head;
					if (!Grammar_unsigned_int())error(0);//这里需要大于0
					if (int_get <= 0)error(0);
					entertab(token_temp, (symbol_temp == INTTK) ? int_array : char_array, int_get, addr, field_flag);
					addr += tab[tab_loc - 1].size;
					getsym(1);
					p = enter_sym_str(p);
					if (symbol != RBRACK)error(13);
					getsym(1);
				}
				else {
					entertab(token_temp, (symbol_temp == INTTK) ? var_int : var_char, NULL, addr, field_flag);
					addr += sizeof(int);
				}
			}
			// 3 好像有一个没有输出
			if (symbol != SEMICN)error(11);
			printf_sym_str(head);
			p = head;
			fprintf(grammar_out, "<变量定义>\n");
			printf("<变量定义>\n");
			p = enter_sym_str(p);
			printf_sym_str(head);
			p = head;
			continue;
		}
		else {
			fprintf(grammar_out, "<变量说明>\n");
			printf("<变量说明>\n");
			if (symbol != RBRACE) {//复合语句变量说明以后就结束
				printf_sym_str(head);
				p = head;
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
	expr_is_char = 0;
	char token_temp[idlen];//缓存当前标识符名
	int j;//返回数组或者函数在tab表的index
	if (Grammar_int() == 1) {
		fprintf(grammar_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 1;
	}
	else if (symbol == LPARENT) {
		getsym(0);
		if (Expression() == 0)error(0);//读取了Expression()的第一个字符,结束时候读取了下一个字符
		expr_is_char = 0;
		print_symbol(symbol);
		if (symbol != RPARENT)error(12);
		fprintf(grammar_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 2;
	}
	else if (symbol == CHARCON) {
		expr_is_char = 1;
		fprintf(grammar_out, "<因子>\n");
		printf("<因子>\n");
		getsym(1);
		return 3;
	}
	else if (symbol == IDENFR) {
		str_cpy(token, token_temp);
		getsym(1);
		if (symbol == LBRACK) {
			print_symbol(symbol);
			getsym(0);
			if (Expression() == 0)error(0);
			if (expr_is_char == 1)error(9);
			print_symbol(symbol);
			if (symbol != RBRACK)error(13);
			j = searchtab(token_temp, num_func);
			if (tab[j].type == char_array)expr_is_char = 1;
			else if (tab[j].type != int_array)error(0);
			fprintf(grammar_out, "<因子>\n");
			printf("<因子>\n");
			getsym(1);
			return 5;
		}
		else if (symbol == LPARENT) {
			j = searchtab(token_temp, num_func);
			if (tab[j].type == return_char_func)expr_is_char = 1;
			else if (tab[j].type != return_int_func)error(0);
			print_symbol(symbol);
			call_fun_Handler(token_temp);
			fprintf(grammar_out, "<因子>\n");
			printf("<因子>\n");
			return 6;
		}
		else {
			j = searchtab(token_temp, num_func);
			if (tab[j].type == const_char || tab[j].type == var_char || tab[j].type == char_para)expr_is_char = 1;
			else if (tab[j].type != const_int && tab[j].type != var_int && tab[j].type != int_para)error(0);
			fprintf(grammar_out, "<因子>\n");
			printf("<因子>\n");
			return 4;
		}
	}
	else return 0;
}
int Term() {
	expr_is_char = 0;
	if (Factor() == 0)return 0;
	while ((symbol == MULT) || (symbol == DIV)) {
		print_symbol(symbol);
		getsym(0);
		if (Factor() == 0)error(0);
		expr_is_char = 0;
	}
	fprintf(grammar_out, "<项>\n");
	printf("<项>\n");
	return 1;
}
/*
结尾symbol没有打印
*/
int Expression() {
	expr_is_char = 0;
	//int first_is_plus_minus = 0;
	if ((symbol == PLUS) || (symbol == MINU)) {
		//first_is_plus_minus = 1;
		getsym(0);
		if (Term()) {
			while ((symbol == PLUS) || (symbol == MINU)) {
				print_symbol(symbol);
				getsym(0);
				if (Term() == 0)error(0);
			}
			fprintf(grammar_out, "<表达式>\n");
			printf("<表达式>\n");
			expr_is_char = 0;
			return 1;
		}
		else error(0);
	}
	else if (Term()) {
		if (symbol != PLUS && symbol != MINU) {
			if (expr_is_char == 1) {
				fprintf(grammar_out, "<表达式>\n");
				printf("<表达式>\n");
				return 1;
			}
		}
		while ((symbol == PLUS) || (symbol == MINU)) {
			print_symbol(symbol);
			getsym(0);
			if (Term() == 0)error(0);
		}
		expr_is_char = 0;
		fprintf(grammar_out, "<表达式>\n");
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
			entertab(token, int_para, NULL, addr, 1);
			getsym(1);
			if (symbol == COMMA) {
				print_symbol(symbol);
				getsym(1);
			}
			else if (symbol != RPARENT)error(12);
		}
		else if (symbol == CHARTK) {
			getsym(0);
			if (symbol != IDENFR)error(0);
			entertab(token, char_para, NULL, addr, 1);
			getsym(1);
			if (symbol == COMMA) {
				print_symbol(symbol);
				getsym(1);
			}
			else if (symbol != RPARENT)error(12);
		}
		else error(0);
	}
	fprintf(grammar_out, "<参数表>\n");
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
	char token_temp[idlen];
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
		if (symbol != SEMICN)error(11);
		getsym(1);
	}
	else if (symbol == PRINTFTK) {
		printf_Handler();
		if (symbol != SEMICN)error(11);
		getsym(1);
	}
	else if (symbol == RETURNTK) {
		return_Handler();
		if (symbol != SEMICN)error(11);
		getsym(1);
	}
	else if (symbol == SEMICN) {
		getsym(1);
	}
	else if (symbol == IDENFR) {
		str_cpy(token, token_temp);
		getsym(0);
		if (symbol == LPARENT) {
			call_fun_Handler(token_temp);
			print_symbol(symbol);
			if (symbol == SEMICN) getsym(1);
			else error(11);
		}
		else {
			assign_Handler(token_temp);
			if (symbol == SEMICN) getsym(1);
			else error(11);
		}
	}
	else error(0);//？这里错误类型可能是11
	fprintf(grammar_out, "<语句>\n");
	printf("<语句>\n");
}
void statement_list() {
	while (symbol != RBRACE) {
		statementHandler();
		if (symbol != RBRACE)print_symbol(symbol);
	}
	fprintf(grammar_out, "<语句列>\n");
	printf("<语句列>\n");
}
/*
复合语句
起始状态读取第一个并没有输出
读取}并输出
*/
void complex_statement(int field_flag) {
	if (symbol != RBRACE)print_symbol(symbol);
	if (symbol == CONSTTK) {
		Const_Declaration(field_flag);
	}
	if (symbol == INTTK || symbol == CHARTK) {
		if (Var_Declaration(field_flag) != 0)error(0);
	}
	statement_list();
	fprintf(grammar_out, "<复合语句>\n");
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
	if (expr_is_char == 1)error(9);
	if (isRelation(symbol)) {
		print_symbol(symbol);
		getsym(0);
		if (Expression() == 0)error(0);
		if (expr_is_char == 1)error(9);
	}
	fprintf(grammar_out, "<条件>\n");
	printf("<条件>\n");
	print_symbol(symbol);
}
void if_Handler() {
	getsym(0);
	if (symbol != LPARENT) error(0);
	getsym(0);
	condition();
	if (symbol != RPARENT)error(12);
	getsym(0);
	statementHandler();
	if (symbol != ELSETK) {
		fprintf(grammar_out, "<条件语句>\n");
		printf("<条件语句>\n");
	}
	else {
		print_symbol(symbol);
		getsym(0);
		statementHandler();
		fprintf(grammar_out, "<条件语句>\n");
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
	if (symbol != RPARENT)error(12);
	getsym(0);
	statementHandler();
	fprintf(grammar_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
void do_while_Handler() {
	getsym(0);
	statementHandler();
	if (symbol != WHILETK)error(14);
	print_symbol(symbol);
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	condition();
	if (symbol != RPARENT)error(12);
	getsym(1);
	fprintf(grammar_out, "<循环语句>\n");
	printf("<循环语句>\n");
}
int step_length() {
	if (symbol == INTCON) {
		fprintf(grammar_out, "<无符号整数>\n");
		printf("<无符号整数>\n");
		fprintf(grammar_out, "<步长>\n");
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
	if (symbol != SEMICN)error(11);
	getsym(0);
	condition();
	if (symbol != SEMICN)error(11);
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
	if (symbol != RPARENT)error(12);
	getsym(0);
	statementHandler();
	fprintf(grammar_out, "<循环语句>\n");
	printf("<循环语句>\n");
}

/*
值参数表
开始状态：预读第一位并没有输出
结束状态：预读下一位)并输出
*/
void value_param_list(int j) {
	j++;
	if (symbol == RPARENT) {
		fprintf(grammar_out, "<值参数表>\n");
		printf("<值参数表>\n");
		print_symbol(symbol);
		return;
	}
	print_symbol(symbol);
	if (Expression()) {
		if (expr_is_char) {
			if (tab[j++].type != char_para)error(5);
		}
		else {
			if (tab[j++].type != int_para)error(5);
		}
		while (symbol == COMMA) {
			print_symbol(symbol);
			getsym(0);
			Expression();
			if (expr_is_char) {
				if (tab[j++].type != char_para)error(5);
			}
			else {
  				if (tab[j++].type != int_para)error(5);
			}
		}
		if (tab[j].type == char_para || tab[j].type == int_para)error(4);
		if (symbol != RPARENT)error(12);
		fprintf(grammar_out, "<值参数表>\n");
		printf("<值参数表>\n");
		print_symbol(symbol);
	}
	else error(0);
}

/*
函数调用
起始状态：(
*/
void call_fun_Handler(char* token_temp) {
	int j = searchtab(token_temp, num_func);
	int j_temp = j;
	getsym(1); 
	value_param_list(j_temp);
	if (tab[j].type == 6 || tab[j].type == 7) {
		fprintf(grammar_out, "<有返回值函数调用语句>\n");
		printf("<有返回值函数调用语句>\n");
	}
	else if (tab[j].type == 8) {
		fprintf(grammar_out, "<无返回值函数调用语句>\n");
		printf("<无返回值函数调用语句>\n");
	}
	else error(0);
	getsym(1);
}
void assign_Handler(char* token_temp) {
	int tab_temp = searchtab(token_temp, num_func);
	if (tab_temp == -1)error(0);
	if (tab[tab_temp].type == 0 || tab[tab_temp].type == 1) {
		error(10);
	}
	if (symbol == ASSIGN) {
		getsym(0);
		if (Expression() == 0)error(0);
		fprintf(grammar_out, "<赋值语句>\n");
		printf("<赋值语句>\n");
		print_symbol(symbol);
	}
	else if (symbol == LBRACK) {
		getsym(0);
		if (Expression() == 0)error(0);
		if (expr_is_char == 1)error(9);
		print_symbol(symbol);
		if (symbol != RBRACK)error(13);
		getsym(0);
		if (symbol != ASSIGN)error(0);
		getsym(0);
		if (Expression() == 0)error(0);
		fprintf(grammar_out, "<赋值语句>\n");
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
	if (symbol != RPARENT)error(12);
	getsym(1);
	fprintf(grammar_out, "<读语句>\n");
	printf("<读语句>\n");
	print_symbol(symbol);
}
void printf_Handler() {
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol == STRCON) {
		fprintf(grammar_out, "<字符串>\n");
		printf("<字符串>\n");
		getsym(0);
		if (symbol == COMMA) {
			getsym(0);
			Expression();
			print_symbol(symbol);
		}
		if (symbol != RPARENT)error(12);
		getsym(1);
		fprintf(grammar_out, "<写语句>\n");
		printf("<写语句>\n");
		print_symbol(symbol);
	}
	else if (Expression()) {
		print_symbol(symbol);
		if (symbol != RPARENT)error(12);
		getsym(1);
		fprintf(grammar_out, "<写语句>\n");
		printf("<写语句>\n");
		print_symbol(symbol);
	}
	else error(0);
}
void return_Handler() {
	getsym(1);
	if (symbol == SEMICN) {
		fprintf(grammar_out, "<返回语句>\n");
		printf("<返回语句>\n");
		print_symbol(symbol);
		return_array[strlen(return_array)] = '2';
		return;
	}
	else if (symbol == LPARENT) {
		print_symbol(symbol);
		getsym(0);
		if (Expression() == 0)error(0);
		print_symbol(symbol);
		if (symbol != RPARENT)error(12);
		getsym(1);
		fprintf(grammar_out, "<返回语句>\n");
		printf("<返回语句>\n");
		print_symbol(symbol);
		return_array[strlen(return_array)] = (expr_is_char == 1)?'1':'0';
	}
	else error(0);
}
void returned_func_definition() {
	enum SYMBOL sym_temp = symbol;
	getsym(0);
	if (symbol != IDENFR)error(0);
	entertab(token,(sym_temp == INTTK) ? return_int_func : return_char_func,NULL, addr, 0);
	num_func++;
	fprintf(grammar_out, "<声明头部>\n");
	printf("<声明头部>\n");
	getsym(0);
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement(1);
	//判断return语句
	return_judge(sym_temp);
	if (symbol != RBRACE)error(0);
	fprintf(grammar_out, "<有返回值函数定义>\n");
	printf("<有返回值函数定义>\n");
	getsym(0);
}
int unreturn_func_definition() {
	getsym(0);
	if (symbol == MAINTK)return 1;
	if (symbol != IDENFR)error(0);
	entertab(token, void_func, NULL, addr, 0);
	num_func++;
	getsym(0);
	if (symbol != LPARENT)error(0);
	paramHandler();
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement(1);
	return_judge(VOIDTK);
	if (symbol != RBRACE)error(0);
	fprintf(grammar_out, "<无返回值函数定义>\n");
	printf("<无返回值函数定义>\n");
	getsym(0);
	return 0;
}
void mainfunc() {
	entertab(token, void_func, NULL, addr, 0);
	num_func++;
	getsym(0);
	if (symbol != LPARENT)error(0);
	getsym(0);
	if (symbol != RPARENT)error(12);
	getsym(0);
	if (symbol != LBRACE)error(0);
	getsym(1);
	complex_statement(0);
	return_judge(VOIDTK);
	if (symbol != RBRACE)error(0);
	fprintf(grammar_out, "<主函数>\n");
	printf("<主函数>\n");
}
void program() {
	getsym(0);
	//if (symbol != CONSTTK)print_symbol(symbol);
	if (symbol == CONSTTK) {
		Const_Declaration(0);
	}
	if (symbol == INTTK || symbol == CHARTK) {
		int temp = Var_Declaration(0);
		if (temp == 1||2) {
			paramHandler();
			if (symbol != LBRACE)error(0);
			getsym(1);
			complex_statement(1);
			return_judge((temp == 1)?INTTK:CHARTK);
			if (symbol != RBRACE)error(0);
			fprintf(grammar_out, "<有返回值函数定义>\n");
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
	fprintf(grammar_out, "<程序>\n");
	printf("<程序>\n");
	getsym(1);
	if (symbol != UNKNOWN)error(0);
}

int main() {
	fp_in = fopen("testfile.txt", "r");
	grammar_out = fopen("output.txt", "w");
	error_out = fopen("error.txt", "w");
	tab_out = fopen("tab.txt", "w");
	program();
	printtab();
	fclose(fp_in);
	fclose(grammar_out);
	fclose(error_out);
	fclose(tab_out);
	return 0;
}