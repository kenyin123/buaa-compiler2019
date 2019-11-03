#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"

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
	while (ch != ';') {
		if (ch == EOF) {
			exit(0);
		}
		ch = getc(fp_in);
	}
	ch = getc(fp_in);
}

void error(int i) {
	printf("error %s line %d\n", errormsg[i],num_line);
	fprintf(error_out, "%d %c\n", num_line, errorsym[i]);
	skip();
	getsym(0);
}