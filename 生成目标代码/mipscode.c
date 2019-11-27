#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"

int num_str = 0;
int Func_loc = 0;
int Para_loc = 0;
int Var_loc = 0;
int Var_addr = 0;

struct Var_info {
	char var_name[idlen];
	int addr;
};
struct Var_info var_tab[info_size];
struct Func_info {
	char func_name[idlen];
	struct Var_info var_tab[info_size];
	int size;
	int num_para;
	int num_var;
};
struct Func_info Func_tab[info_size];

int str_is_num(char* str) {
	if (str[0] == '+' || str[0] == '-' || (str[0] <= '9') && (str[0] >= '0')) {
		return 1;
	}
	return 0;
}

/*
如果是函数内变量，返回index，如果不是，返回-1
*/
int get_index(char* id, int func_num, int* addr) {
	int i = 0;
	for (i = 0; i < Func_tab[func_num].num_var; i++) {
		if (strcmp(Func_tab[func_num].var_tab[i].var_name, id) == 0) {
			*addr = Func_tab[func_num].var_tab[i].addr;
			return i;
		}
	}
	return -1;
}
char* str_name_gen() {
	static char str_name[idlen];
	sprintf(str_name, "$str%d", num_str++);
	return str_name;
}

void PLUS_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op2 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
	}

	if (op1 != -1) {
		if (op2 != -1) {
			fprintf(mips_out, "\taddu $t0, $s%d, $s%d\n", op1, op2);
		}
		else {
			fprintf(mips_out, "\taddu $t0, $s%d, $t1\n", op1);
		}
	}
	else {
		if (op2 != -1) {
			fprintf(mips_out, "\taddu $t0, $t0, $s%d\n", op2);
		}
		else {
			fprintf(mips_out, "\taddu $t0, $t0, $t1\n");
		}
	}


	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $t0, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $t0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
}
void MINU_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op2 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
	}

	if (op1 != -1) {
		if (op2 != -1) {
			fprintf(mips_out, "\tsub $t0, $s%d,$s%d\n", op1, op2);
		}
		else {
			fprintf(mips_out, "\tsub $t0, $s%d,$t1\n", op1);
		}
	}
	else {
		if (op2 != -1) {
			fprintf(mips_out, "\tsub $t0, $t0,$s%d\n", op2);
		}
		else {
			fprintf(mips_out, "\tsub $t0, $t0,$t1\n");
		}
	}


	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $t0, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $t0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
}
void MULT_Hadnler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op2 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
	}

	if (op1 != -1) {
		if (op2 != -1) {
			fprintf(mips_out, "\tmul $t0, $s%d,$s%d\n", op1, op2);
		}
		else {
			fprintf(mips_out, "\tmul $t0, $s%d,$t1\n", op1);
		}
	}
	else {
		if (op2 != -1) {
			fprintf(mips_out, "\tmul $t0, $t0,$s%d\n", op2);
		}
		else {
			fprintf(mips_out, "\tmul $t0, $t0,$t1\n");
		}
	}


	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $t0, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $t0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
}
void DIV_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op2 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
	}

	if (op1 != -1) {
		if (op2 != -1) {
			fprintf(mips_out, "\tdiv $t0, $s%d,$s%d\n", op1, op2);
		}
		else {
			fprintf(mips_out, "\tdiv $t0, $s%d,$t1\n", op1);
		}
	}
	else {
		if (op2 != -1) {
			fprintf(mips_out, "\tdiv $t0, $t0,$s%d\n", op2);
		}
		else {
			fprintf(mips_out, "\tdiv $t0, $t0,$t1\n");
		}
	}


	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $t0, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $t0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
	}
}

void Func_Handler(int i) {
	int addr_temp = 0;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	if (strcmp(midcode[i].argu1, "main") == 0) {
		fprintf(mips_out, "main:\n");
		fprintf(mips_out, "\tmove $fp, $sp\n");
		fprintf(mips_out, "#退栈空间\n\tsub $sp, $sp, %d\n",Func_tab[Func_loc].size);
		//fp暂时静止，sp是动的
	}
	else {
		fprintf(mips_out, "%s:\n",midcode[i].argu1);
		fprintf(mips_out, "#为函数局部变量和临时变量退栈空间，保存s系寄存器和$ra,$fp\n");
		fprintf(mips_out, "\tsub $sp, $sp, %d\n", Func_tab[Func_loc].size - Func_tab[Func_loc].num_para * 4 + 40);
		for (int j = 0; j < Func_tab[Func_loc].num_var - Func_tab[Func_loc].num_para && j < 8; j++) {
			fprintf(mips_out, "\tsw $s%d, %d($sp)\n", j, 40 - j * 4);
		}
		fprintf(mips_out, "\tsw $ra, 8($sp)\n\tsw $fp, 4($sp)\n");
		fprintf(mips_out, "#fp为被调用者基地址\n\tadd $fp, $sp, %d\n", Func_tab[Func_loc].size + 40);
	}
}
void Call_Handler(int i) {
	fprintf(mips_out, "\tjal %s\n",midcode[i].argu1);
}
void Push_Handler(int i) {
	int addr_temp = 0;
	int index_temp = -1;
	int op1 = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	if (op1 == -1) {
		fprintf(mips_out, "\tsw $t0, 0($sp)\n\tsub $sp, $sp, 4\n");
	}
	else {
		fprintf(mips_out, "\tsw $s%d, 0($sp)\n\tsub $sp, $sp, 4\n", op1);
	}
}
void Func_Ret_Handler(int i) {
	int addr_temp = 0;
	int index_temp = -1;
	//char str_temp[idlen];
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $v0\n",index_temp - ParaNum);
	}
	else {
		fprintf(mips_out, "\tsw $v0, -%d($fp)\n",addr_temp);//?
	}
}
void VarAssign_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//如果argu1是数字
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);//?
		}
	}
	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		if (op1 == -1) {
			fprintf(mips_out, "\tsw $t0, %s\n", midcode[i].result);
		}
		else {
			fprintf(mips_out, "\tsw $s%d, %s\n", op1,midcode[i].result);
		}
	}
	else if (index_temp < ParaNum) {
		if (op1 == -1) {
			fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);
		}
		else {
			fprintf(mips_out, "\tsw $s%d, -%d($fp)\n",op1, addr_temp);//?
		}
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		if (op1 == -1) {
			fprintf(mips_out, "\tmove $s%d, $t0\n", index_temp - ParaNum);
		}
		else {
			fprintf(mips_out, "\tmove $s%d, $s%d\n", index_temp - ParaNum,op1);
		}
	}
	else {//变量，不在s系寄存器
		if (op1 == -1) {
			fprintf(mips_out, "\tsw $t0, -%d($fp)\n", addr_temp);//?
		}
		else {
			fprintf(mips_out, "\tsw $s%d, -%d($fp)\n",op1, addr_temp);//?
		}
	}
}
void Array_Assign_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	int global_flag = 0;	
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	//t0存数组地址
	if (index_temp == -1) {
		global_flag = 1;
		fprintf(mips_out, "\tla $t0, %s\n", midcode[i].result);
	}
	else {
		fprintf(mips_out, "\tla $t0, -%d($fp)\n", addr_temp);
	}
	//t1存偏移
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);//?
		}
	}
	fprintf(mips_out, "\tli $t2, 4\n");
	if (op1 == -1) {
		fprintf(mips_out, "\tmul $t1, $t1, $t2\n");
	}
	else {
		fprintf(mips_out, "\tmul $t2, $s%d, $t2\n",op1);
	}
	//array[t1] = t2
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t2, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t2, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t2, -%d($fp)\n", addr_temp);
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op2 = index_temp - ParaNum;
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $t2, -%d($fp)\n", addr_temp);
		}
	}
	if (global_flag) {
		fprintf(mips_out, "\tadd $t0, $t0, $t1\n");
	}
	else {
		fprintf(mips_out, "\tsub $t0, $t0, $t1\n");
	}
	if (op2 == -1) {
		fprintf(mips_out, "\tsw $t2, 0($t0)\n");
	}
	else {
		fprintf(mips_out, "\tsw $s%d, 0($t0)\n",op2);
	}
}
void Array_Access_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	int global_flag = 0;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
	//t0数组名
	if (index_temp == -1) {
		global_flag = 1;
		fprintf(mips_out, "\tla $t0, %s\n", midcode[i].argu1);
	}
	else {
		fprintf(mips_out, "\tla $t0, -%d($fp)\n", addr_temp);
	}
	//t1下标
	if (str_is_num(midcode[i].argu2)) {
		fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
	}
	else {
		index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			op1 = index_temp - ParaNum;
		}
		else {
			fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
		}
	}
	fprintf(mips_out, "\tli $t2, 4\n");
	if (op1 == -1) {
		fprintf(mips_out, "\tmul $t1, $t1, $t2\n");
	}
	else {
		fprintf(mips_out, "\tmul $t2, $s%d, $t2\n", op1);
	}
	//t2 = array[t1]
	if (global_flag) {
		fprintf(mips_out, "\tadd $t0, $t0, $t1\n");
	}
	else {
		fprintf(mips_out, "\tsub $t0, $t0, $t1\n");
	}
	index_temp = get_index(midcode[i].result, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tlw $t2, 0($t0)\n");
		fprintf(mips_out, "\tsw $t2, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tlw $t2, 0($t0)\n");
		fprintf(mips_out, "\tsw $t2, -%d($fp)\n", addr_temp);

	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tlw $s%d, 0($t0)\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tlw $t2, 0($t0)\n");
		fprintf(mips_out, "\tsw $t2, -%d($fp)\n", addr_temp);
	}

}
void Condition_Hanlder(int i) {
	int addr_temp = 0;
	int index_temp;
	int op1 = -1;
	int op2 = -1;
	int ParaNum = Func_tab[Func_loc].num_para;
	int bz_flag = (midcode[i + 1].type == BZ) ? 1 : 0;
	if (midcode[i].type == LSSOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}
		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbge $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbge $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbge $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbge $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tblt $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tblt $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tblt $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tblt $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		
	}
	else if (midcode[i].type == LEQOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}
		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbgt $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbgt $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbgt $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbgt $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tble $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tble $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tble $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tble $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		
	}
	else if (midcode[i].type == GREOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}
		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tble $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tble $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tble $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tble $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbgt $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbgt $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbgt $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbgt $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		
		}
		
	}
	else if (midcode[i].type == GEQOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}
		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tblt $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tblt $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tblt $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tblt $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbge $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbge $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbge $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbge $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		
		}
		
	}
	else if (midcode[i].type == EQLOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}
		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbne $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbne $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbne $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbne $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbeq $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbeq $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbeq $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbeq $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		
		}
		
	}
	else if (midcode[i].type == NEQOP) {
		if (str_is_num(midcode[i].argu1)) {
			fprintf(mips_out, "\tli $t0, %s\n", midcode[i].argu1);
		}
		else {
			index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t0, %s\n", midcode[i].argu1);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op1 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t0, -%d($fp)\n", addr_temp);
			}
		}
		if (str_is_num(midcode[i].argu2)) {
			fprintf(mips_out, "\tli $t1, %s\n", midcode[i].argu2);
		}
		else {
			index_temp = get_index(midcode[i].argu2, Func_loc, &addr_temp);
			if (index_temp == -1) {
				fprintf(mips_out, "\tlw $t1, %s\n", midcode[i].argu2);
			}
			else if (index_temp < ParaNum) {
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
			else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
				op2 = index_temp - ParaNum;
			}
			else {//变量，不在s系寄存器
				fprintf(mips_out, "\tlw $t1, -%d($fp)\n", addr_temp);
			}
		}

		if (bz_flag) {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbeq $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbeq $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbeq $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbeq $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}
		}
		else {
			if (op1 == -1) {
				if (op2 == -1) {
					fprintf(mips_out, "\tbne $t0, $t1, %s\n", midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbne $t0, $s%d, %s\n", op2, midcode[i + 1].argu1);
				}
			}
			else {
				if (op2 == -1) {
					fprintf(mips_out, "\tbne $s%d, $t1, %s\n", op1, midcode[i + 1].argu1);
				}
				else {
					fprintf(mips_out, "\tbne $s%d, $s%d, %s\n", op1, op2, midcode[i + 1].argu1);
				}
			}

		}
	}
}
void SetLabel_Handler(int i) {
	fprintf(mips_out, "%s:\n",midcode[i].argu1);
}
void Goto_Handler(int i) {
	fprintf(mips_out, "\tj %s\n", midcode[i].argu1);
}
void Scanf_int_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	fprintf(mips_out, "#读取int\n\tli $v0, 5\n");
	fprintf(mips_out, "\tsyscall \n");

	//如果argu1是数字
	index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $v0, %s\n", midcode[i].argu1);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $v0, -%d($fp)\n", addr_temp);
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $v0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $v0, -%d($fp)\n", addr_temp);
	}
}
void Scnaf_char_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	fprintf(mips_out, "#读取char\n\tli $v0, 12\n");
	fprintf(mips_out, "\tsyscall \n");

	//如果argu1是数字
	index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
	if (index_temp == -1) {
		fprintf(mips_out, "\tsw $v0, %s\n", midcode[i].result);
	}
	else if (index_temp < ParaNum) {
		fprintf(mips_out, "\tsw $v0, -%d($fp)\n", addr_temp);
	}
	else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
		fprintf(mips_out, "\tmove $s%d, $v0\n", index_temp - ParaNum);
	}
	else {//变量，不在s系寄存器
		fprintf(mips_out, "\tsw $v0, -%d($fp)\n", addr_temp);
	}
}
void Print_int_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数

	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "#打印int\n\tli $a0, %s\n", midcode[i].argu1);
		fprintf(mips_out, "\tli $v0, 1\n\tsyscall\n");
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "#打印int\n\tlw $a0, %s\n", midcode[i].argu1);
			fprintf(mips_out, "\tli $v0, 1\n\tsyscall\n");
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "#打印int\n\tlw $a0, -%d($fp)\n", addr_temp);
			fprintf(mips_out, "\tli $v0, 1\n\tsyscall\n");
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			fprintf(mips_out, "#打印int\n\tmove $a0, $s%d\n", index_temp - ParaNum);
			fprintf(mips_out, "\tli $v0, 1\n\tsyscall\n");
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "#打印int\n\tlw $a0, -%d($fp)\n", addr_temp);
			fprintf(mips_out, "\tli $v0, 1\n\tsyscall\n");
		}
	}

}
void Print_char_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数

	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "#打印char\n\tli $a0, %s\n", midcode[i].argu1);
		fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "#打印char\n\tlw $a0, %s\n", midcode[i].argu1);
			fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "#打印char\n\tlw $a0, -%d($fp)\n", addr_temp);
			fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			fprintf(mips_out, "#打印char\n\tmove $a0, $s%d\n", index_temp - ParaNum);
			fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "#打印char\n\tlw $a0, -%d($fp)\n", addr_temp);
			fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
		}
	}
}
void Print_str_Handler(int i) {
	
	fprintf(mips_out, "#打印str\n\tla $a0, %s\n",midcode[i].argu1);//?
	fprintf(mips_out, "\tli $v0, 4\n\tsyscall\n");
}
void Ret_Expr_Handler(int i) {
	int addr_temp = 0;
	int index_temp;
	int ParaNum = Func_tab[Func_loc].num_para;//当前函数的参数个数
	//v0保存返回值
	if (str_is_num(midcode[i].argu1)) {
		fprintf(mips_out, "\tli $v0, %s\n", midcode[i].argu1);
	}
	else {
		index_temp = get_index(midcode[i].argu1, Func_loc, &addr_temp);
		if (index_temp == -1) {
			fprintf(mips_out, "\tlw $v0, %s\n", midcode[i].argu1);
		}
		else if (index_temp < ParaNum) {
			fprintf(mips_out, "\tlw $v0, -%d($fp)\n", addr_temp);
		}
		else if (index_temp >= ParaNum && index_temp <= ParaNum + 7) {
			fprintf(mips_out, "\tmove $v0, $s%d\n", index_temp - ParaNum);
		}
		else {//变量，不在s系寄存器
			fprintf(mips_out, "\tlw $v0, -%d($fp)\n", addr_temp);
		}
	}
	fprintf(mips_out, "\tlw $fp, 4($sp)\n\tlw $ra, 8($sp)\n");
	for (int w = 0; w < Func_tab[Func_loc].num_var - Func_tab[Func_loc].num_para && w < 8; w++) {
		fprintf(mips_out, "\tlw $s%d, %d($sp)\n", w,40-w*4);
	}
	fprintf(mips_out, "\tadd $sp, $sp, %d\n", Func_tab[Func_loc].size+40);
	fprintf(mips_out, "\tjr $ra\n");
}
void Ret_null_Handler(int i) {
	if (Func_loc == num_func - 1) {
		//main函数
		fprintf(mips_out, "\tli $v0, 10\n\tsyscall\n");
	}
	else {
		fprintf(mips_out, "\tlw $fp, 4($sp)\n\tlw $ra, 8($sp)\n");
		for (int w = 0; w < Func_tab[Func_loc].num_var - Func_tab[Func_loc].num_para && w < 8; w++) {
			fprintf(mips_out, "\tlw $s%d, %d($sp)\n", w, 40 - w * 4);
		}
		fprintf(mips_out, "\tadd $sp, $sp, %d\n", Func_tab[Func_loc].size + 40);
		fprintf(mips_out, "\tjr $ra\n");
	}
}

void Print_newline() {
	fprintf(mips_out, "#换行\n\tli $a0, 10\n");
	fprintf(mips_out, "\tli $v0, 11\n\tsyscall\n");
}
void mips() {
	int i = 0;
	int j = 0;
	fprintf(mips_out, ".data\n");
	char str_name[idlen];
	for (i = 0; i < tab_loc; i++) {
		if (tab[i].type == var_int || tab[i].type == var_char) {
			fprintf(mips_out, "\t%s:.word 0\n", tab[i].id);
		}
		else if (tab[i].type == array_int || tab[i].type == array_char) {
			fprintf(mips_out, "\t%s:.word 0:%d\n", tab[i].id, 4 * tab[i].value);
		}
		if (tab[i].type >= 6 && tab[i].type <= 8)break;
	}
	/*
		把字符串写进data，argu1改名
	*/
	for (i = 0; i < midcode_loc; i++) {
		if (midcode[i].type == PRINT_STR) {
			str_cpy( str_name_gen(),str_name);
			fprintf(mips_out, "\t%s:.asciiz \"%s\"\n",str_name, midcode[i].argu1);
			str_cpy(str_name, midcode[i].argu1);
		}
	}
	/*
		函数信息表，变量表初始化
		变量只有函数内定义的变量和$开头的临时变量
	*/
	for (i = 0; i < midcode_loc; i++) {
		if (midcode[i].type >= 10 && midcode[i].type <= 12) {//函数
			Para_loc = 0;
			Var_loc = 0;
			Var_addr = 0;
			str_cpy(midcode[i].argu1, Func_tab[Func_loc].func_name);
			for (j = i + 1; j < midcode_loc; j++) {
				if ((midcode[j].type >= 10 && midcode[j].type <= 12) || j + 1 == midcode_loc) {
					Func_tab[Func_loc].size = Var_addr;
					Func_tab[Func_loc].num_para = Para_loc;
					Func_tab[Func_loc].num_var = Var_loc;
					Func_loc++;
					break;
				}
				if (midcode[j].type == VAR_INT || midcode[j].type == VAR_CHAR || midcode[j].type == PARA_INT || midcode[j].type == PARA_CHAR) {
					if (midcode[j].type == PARA_INT || midcode[j].type == PARA_CHAR) {
						Para_loc++;
					}
					str_cpy(midcode[j].argu1, Func_tab[Func_loc].var_tab[Var_loc].var_name);
					Func_tab[Func_loc].var_tab[Var_loc].addr = Var_addr;
					Var_loc++;
					Var_addr += 4;
				}
				else if (midcode[j].type == ARRAY_INT || midcode[j].type == ARRAY_CHAR) {
					str_cpy(midcode[j].argu1, Func_tab[Func_loc].var_tab[Var_loc].var_name);
					Func_tab[Func_loc].var_tab[Var_loc].addr = Var_addr;
					Var_loc++;
					Var_addr += 4 * midcode[j].value;
				}
				else if (midcode[j].type != DEL && midcode[j].result[0] == '$') {
					str_cpy(midcode[j].result, Func_tab[Func_loc].var_tab[Var_loc].var_name);
					Func_tab[Func_loc].var_tab[Var_loc].addr = Var_addr;
					Var_loc++;
					Var_addr += 4;
				}
			}
		}
	}
	Func_loc = -1;
	fprintf(mips_out, ".text\n\tj main\n");
	for (i = 0; i < midcode_loc; i++) {
		switch (midcode[i].type) {
		case PLUSOP:
			PLUS_Handler(i);
			break;
		case MINUOP:
			MINU_Handler(i);
			break;
		case MULTOP:
			MULT_Hadnler(i);
			break;
		case DIVOP:
			DIV_Handler(i);
			break;
		case FUNC_INT:
			Func_loc++;
			Func_Handler(i);
			break;
		case FUNC_CHAR:
			Func_loc++;
			Func_Handler(i);
			break;
		case FUNC_VOID:
			Func_loc++;
			Func_Handler(i);
			break;
		case CALL:
			Call_Handler(i);
			break;
		case PUSH:
			Push_Handler(i);
			break;
		case FUNC_RET:
			Func_Ret_Handler(i);
			break;
		case VAR_ASSIGN:
			VarAssign_Handler(i);
			break;
		case ARRAY_ASSIGN:
			Array_Assign_Handler(i);
			break;
		case ARRAY_ACCESS:
			Array_Access_Handler(i);
			break;
		case LSSOP:
			Condition_Hanlder(i);
			break;
		case LEQOP:
			Condition_Hanlder(i);
			break;
		case GREOP:
			Condition_Hanlder(i);
			break;
		case GEQOP:
			Condition_Hanlder(i);
			break;
		case EQLOP:
			Condition_Hanlder(i);
			break;
		case NEQOP:
			Condition_Hanlder(i);
			break;
		case SETLABEL:
			SetLabel_Handler(i);
			break;
		case GOTO:
			Goto_Handler(i);
			break;
		case SCANF_INT:
			Scanf_int_Handler(i);
			break;
		case SCANF_CHAR:
			Scnaf_char_Handler(i);
			break;
		case PRINT_INT:
			Print_int_Handler(i);
			break;
		case PRINT_CHAR:
			Print_char_Handler(i);
			break;
		case PRINT_STR:
			Print_str_Handler(i);
			break;
		case RET_EXPR:
			Ret_Expr_Handler(i);
			break;
		case RET_NULL:
			Ret_null_Handler(i);
			break;
		case PRINT_NEWLINE:
			Print_newline();
			break;
		default:
			//printf("%d\n",midcode[i].type);
			break;
		}
	}
}