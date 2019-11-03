#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"
enum SYMBOL symbol_error_klmn = UNKNOWN;
int error_klmn = 0;
int error_a = 0;
/*
0:int
1:char
2:return;
*/
char return_array[return_array_len] = { '\0' };

char errorsym[20] = "0abcdefghijklmno";
char errormsg[20][100] = {
"未分类错误",
"非法符号或不符合词法",
"名字重定义",
"未定义的名字",
"函数参数个数不匹配",
"函数参数类型不匹配",
"条件判断中出现不合法的类型",
"无返回值的函数存在不匹配的return语句",
"有返回值的函数缺少return语句或存在不匹配的return语句",
"数组元素的下标只能是整型表达式",
"不能改变常量的值",
"应为分号",
"应为)",
"应为]",
"do-while语句缺少while",
"常量定义中 = 后面只能是整型或字符型常量"
};


void skip() {
	/*
	while (ch != ';') {
		if (ch == EOF) {
			printf("EOF");
			exit(0);
		}
		ch = getc(fp_in);
	}
	*/
	while (!isspace(ch)) {
		if (ch == EOF) {
			printf("EOF");
			exit(0);
		}
		ch = getc(fp_in);
	}
}

void error(int i) {
	if (i >= 11 && i <= 14) {
		error_klmn = 1;
		symbol_error_klmn = symbol;
		switch (i) {
		case 11:
			symbol = SEMICN; 
			break;
		case 12:
			symbol = RPARENT; 
			break;
		case 13:
			symbol = RBRACK; 
			break;
		case 14:
			symbol = WHILETK; 
			break;
		}
		if (i != 14) {
			printf("error %s line %d\n", errormsg[i], num_line_temp);
			fprintf(error_out, "%d %c\n", num_line_temp, errorsym[i]);
		}
		else {
			printf("error %s line %d\n", errormsg[i], num_line);
			fprintf(error_out, "%d %c\n", num_line, errorsym[i]);
		}
		
	}
	else {
		printf("error %s line %d\n", errormsg[i], num_line);
		fprintf(error_out, "%d %c\n", num_line, errorsym[i]);
	}
	//skip();
	//getsym(0);
}
void return_judge(enum SYMBOL sym) {
	if (strlen(return_array) == 0) {
		if (sym == INTTK || sym == CHARTK) {
			error(8);
		}
	}
	else if (sym == INTTK) {
		for (int i = 0; i < strlen(return_array); i++) {
			if (return_array[i] == '0')continue;
			else if (return_array[i] == '1' || return_array[i] == '2') {
				error(8);
			}
		}
	}
	else if (sym == CHARTK) {
		for (int i = 0; i < strlen(return_array); i++) {
			if (return_array[i] == '1')continue;
			else if (return_array[i] == '0' || return_array[i] == '2') {
				error(8);
			}
		}
	}
	else if (sym == VOIDTK) {
		for (int i = 0; i < strlen(return_array); i++) {
			if (return_array[i] == '2')continue;
			else if (return_array[i] == '0' || return_array[i] == '1') {
				error(7);
			}
		}
	}
	for (int i = 0; i < return_array_len; i++) {
		return_array[i] = '\0';
	}
}