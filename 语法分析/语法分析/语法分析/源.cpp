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
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (symbol == MINU) {
		getsym();
		if (Grammar_unsigned_int()) {
			printf("<整数>\n");
			return 1;
		}
		else error(0);
	}
	else if (Grammar_unsigned_int()) {
		printf("<整数>\n");
		return 1;
	}
	else return 0;
}

//起始状态：读取到第一个字符
//结束状态：读到";"
//有常量定义的话返回1.没有返回0
int Const_Definition() {
	if (symbol == INTCON) {
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
		printf("<常量定义>");
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
	printf("<常量说明>\n");
	return 0;
}


//需要和有返回值函数定义区分
//起始状态：读取了第一个字符并未识别，为判断属于哪个语法成分
//开头字符可能情况：int char void
//返回值：1int 2char 3void
/*结束状态：
return 3:主函数或者无返回函数定义,读到void
return 2:有返回函数定义，读到(
return 1：读到";"后一位
*/
int Var_Definition() {
	if (symbol == VOIDTK)return 3;//主函数或者无返回函数定义
	if (symbol == CHARTK) {
		getsym();
		if (symbol != IDENFR)error(0);
		getsym();

		//char a;
		//char a[2];
		//char a()
		//char a,b[2];
		//这里如果不是数组，那么就不会触发进入if
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
		printf("<变量定义>\n");
	}
	else error(0);
	getsym();
	return 1;
}

int Var_Declaration(){
	int temp_i = Var_Definition();
	int flag = 0;
	if (temp_i == 1)flag = 1;
	while (temp_i == 1) {
		temp_i = Var_Definition();
	}
	if (flag == 1) {
		printf("<变量说明>\n");
	}
	if (temp_i == 2) return 2;
	if (temp_i == 3) return 3;
}
//起始状态：读取了第一个字符。
//判断这里是因子的话，就应该是因子，不会有别的可能
int Factor() {
	if (symbol == LPARENT) {
		getsym();
		Expression();//读取了Expression()的第一个字符,结束时候读取了下一个字符
		if (symbol != RPARENT)error(0);
		getsym();
		return 0;
	}
	if (symbol == PLUS) {
		getsym();
		if (symbol != UNSIGNED_INT)error(0);
		getsym();
		return 0;
	}
	if (symbol == MINUS) {
		getsym();
		if (symbol != UNSIGNED_INT)error(0);
		getsym();
		return 0;
	}
	//修改一下整数的问题吧
}
int Term() {}
int Expression() {}



int Condition_Statement() {
	if (symbol != IFSY)error(0);
	getsym();

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

		case 40:break;
		case 41:break;
		case 42:break;
		}
		ch = fgetc(fp);
	}

	system("pause");
	return 0;

}