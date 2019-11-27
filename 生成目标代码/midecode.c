#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"
/*
符号表
struct str_tab {
	char id[idlen];
	int type;
	int value;
	int addr;
	int size;
	int lev;
};
*/
struct str_tab tab[tab_size];
int type = 0;
int value = 0;
int addr = 0;
int lev = 0;
int tab_loc = 0;//当前在tab中间的位置,即符号表长度

/*
struct str_midcode {
	int type;
	char argu1[idlen];
	char argu2[idlen];
	char result[idlen];
	int value;//常量的值或者数组的长度
};
*/
struct str_midcode midcode[midecode_size];
int midcode_loc = 0;
int id_name_num = 0;//临时变量名计数
int label_name_num = 0;

//a复制给给b
void str_cpy(char* str1, char* str2) {
	int i;
	for (i = 0; str1[i]!='\0'; i++) {
		str2[i] = str1[i];
	}
	str2[i] = '\0';
}

void entertab(char* id, int type, int value, int addr, int lev){
	if (tab_loc > tab_size) {
		error(0);
	}
	int i;
	if (lev == 0) {
		for (i = 0; i < tab_loc; i++) {
			if (tab[i].lev == 0) {
				if (strcmp(tab[i].id, id) == 0) {
					error(2);
					return;
				}
			}
		}
	}
	else {
		for (i = tab_loc - 1; tab[i].type != 6 && tab[i].type != 7 && tab[i].type != 8; i--) {
			if (strcmp(tab[i].id, id) == 0) {
				error(2);
				return;
			}
		}
	}
	str_cpy(id, tab[tab_loc].id);
	tab[tab_loc].type = type;
	tab[tab_loc].value = value;
	if ((type >= 0 && type <= 3) || (type >= 9 && type <= 10)) {
		tab[tab_loc].size = sizeof(int);
	}
	else if (type == 4 || type == 5) {
		tab[tab_loc].size = value*sizeof(int);
	}
	tab[tab_loc].addr = addr;
	tab[tab_loc].lev = lev;
	tab_loc++;
}
/*
没找到的话返回-1
*/
int searchtab(char* ident,int num_func) {
	int i;
	int num_func_temp = 0;
	for (i = 0; i < tab_loc; i++) {
		if (tab[i].type >= 6 && tab[i].type <= 8) {
			num_func_temp++;
		}
		if (num_func_temp == num_func)break;
	}
	i++;
	int j;
	int flag_found = 0;
	//先从函数内部找
	for (j = i;  !(tab[j].type >= 6 && tab[j].type <= 8) && j < tab_loc; j++) {
		if (strcmp(ident, tab[j].id) == 0) {
			flag_found = 1;
			break;
		}
	}
	if (flag_found)return j;
	//回溯查找
	for (j = i - 1; j >= 0; j--) {
		if (!(tab[j].type >= 6 && tab[j].type <= 8))continue;
		if ((tab[j].type >= 6 && tab[j].type <= 8) && (strcmp(ident, tab[j].id) == 0)) {
			flag_found = 1;
			break;
		}
	}
	if (flag_found)return j;
	//全局变量
	for (j = 0; !(tab[j].type >= 6 && tab[j].type <= 8); j++) {
		if (strcmp(ident, tab[j].id) == 0) {
			flag_found = 1;
			break;
		}
	}
	if (flag_found)return j;
	error(3);
	return -1;
}

void printtab() {
	char type[20][20] = { 
		"const_int" ,"const_char" ,
		"var_int" ,"var_char" ,
		"array_int" , "array_char" ,
		"return_int_func" ,"return_char_func" ,
		"void_func",
		"int_para","char_para" 
	};
	for (int i = 0; i < tab_loc; i++) {
		fprintf(tab_out, "%s %s %d\n", type[tab[i].type], tab[i].id, tab[i].lev);
	}
}

void insert_midcode(int type, char* argu1, char* argu2, char* result, int value) {
	midcode[midcode_loc].type = type;
	if (argu1 != NULL) {
		str_cpy(argu1, midcode[midcode_loc].argu1);
	}
	else {
		str_cpy("", midcode[midcode_loc].argu1);
	}

	if (argu2 != NULL) {
		str_cpy(argu2, midcode[midcode_loc].argu2);
	}
	else {
		str_cpy("", midcode[midcode_loc].argu2);
	}

	if (result != NULL) {
		str_cpy(result, midcode[midcode_loc].result);
	}
	else {
		str_cpy("", midcode[midcode_loc].result);
	}

	midcode[midcode_loc].value = value;
	midcode_loc++;
}
void print_midcode() {
	int i;
	char temp[idlen];
	for (i = 0; i < midcode_loc; i++) {
		switch (midcode[i].type) {
		case CONST_INT: {
			fprintf(midcode_out, "const int %s = %d\n", midcode[i].argu1, midcode[i].value);
			break;
		}
		case CONST_CHAR: {
			fprintf(midcode_out, "const char %s = '%c'\n", midcode[i].argu1, midcode[i].value);
			break;
		}
		case VAR_INT: {
			fprintf(midcode_out, "var int %s\n", midcode[i].argu1);
			break;
		}
		case VAR_CHAR: {
			fprintf(midcode_out, "var char %s\n", midcode[i].argu1);
			break;
		}
		case ARRAY_INT: {
			fprintf(midcode_out, "int %s[%d]\n", midcode[i].argu1, midcode[i].value);
			break;
		}
		case ARRAY_CHAR: {
			fprintf(midcode_out, "char %s[%d]\n", midcode[i].argu1, midcode[i].value);
			break;
		}
		case PLUSOP: {
			fprintf(midcode_out, "%s = %s + %s\n", midcode[i].result, midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case MINUOP: {
			fprintf(midcode_out, "%s = %s - %s\n", midcode[i].result, midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case MULTOP: {
			fprintf(midcode_out, "%s = %s * %s\n", midcode[i].result, midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case DIVOP: {
			fprintf(midcode_out, "%s = %s / %s\n", midcode[i].result, midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case FUNC_INT: {
			fprintf(midcode_out, "int %s()\n", midcode[i].argu1);
			break;
		}
		case FUNC_CHAR: {
			fprintf(midcode_out, "char %s()\n", midcode[i].argu1);
			break;
		}
		case FUNC_VOID: {
			fprintf(midcode_out, "void %s()\n", midcode[i].argu1);
			break;
		}
		case PARA_INT: {
			fprintf(midcode_out, "para int %s\n", midcode[i].argu1);
			break;
		}
		case PARA_CHAR: {
			fprintf(midcode_out, "para char %s\n", midcode[i].argu1);
			break;
		}
		case CALL: {
			fprintf(midcode_out, "call %s\n", midcode[i].argu1);
			break;
		}
		case PUSH: {
			fprintf(midcode_out, "push %s\n", midcode[i].argu1);
			break;
		}
		case FUNC_RET: {
			fprintf(midcode_out, "%s = RET\n", midcode[i].result);
			break;
		}
		case VAR_ASSIGN: {
			fprintf(midcode_out, "%s = %s\n", midcode[i].result, midcode[i].argu1);
			break;
		}
		case ARRAY_ASSIGN: {
			fprintf(midcode_out, "%s[%s] = %s\n", midcode[i].result, midcode[i].argu2, midcode[i].argu1);
			break;
		}
		case ARRAY_ACCESS: {
			fprintf(midcode_out, "%s = %s[%s]\n", midcode[i].result, midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case GREOP: {
			fprintf(midcode_out, "%s > %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case LSSOP: {
			fprintf(midcode_out, "%s < %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case GEQOP: {
			fprintf(midcode_out, "%s >= %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case LEQOP: {
			fprintf(midcode_out, "%s <= %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case EQLOP: {
			fprintf(midcode_out, "%s == %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case NEQOP: {
			fprintf(midcode_out, "%s != %s\n", midcode[i].argu1, midcode[i].argu2);
			break;
		}
		case SETLABEL: {
			fprintf(midcode_out, "%s:\n", midcode[i].argu1);
			break;
		}
		case GOTO: {
			fprintf(midcode_out, "GOTO %s\n", midcode[i].argu1);
			break;
		}
		case BNZ: {
			fprintf(midcode_out, "BNZ %s\n", midcode[i].argu1);
			break;
		}
		case BZ: {
			fprintf(midcode_out, "BZ %s\n", midcode[i].argu1);
			break;
		}
		case SCANF_INT: {
			fprintf(midcode_out, "scanf(%s)\n", midcode[i].argu1);
			break;
		}
		case SCANF_CHAR: {
			fprintf(midcode_out, "scanf(%s)\n", midcode[i].argu1);
			break;
		}
		case PRINT_INT: {
			fprintf(midcode_out, "printf(%s)\n", midcode[i].argu1);
			break;
		}
		case PRINT_CHAR: {
			fprintf(midcode_out, "printf(%s)\n", midcode[i].argu1);
			break;
		}
		case PRINT_STR: {
			fprintf(midcode_out, "printf(\"%s\")\n", midcode[i].argu1);
			break;
		}
		case RET_EXPR: {
			fprintf(midcode_out, "ret %s\n", midcode[i].argu1);
			break;
		}
		case RET_NULL: {
			fprintf(midcode_out, "return null\n");
			break;
		}
		case PRINT_NEWLINE: {
			break;
		}
		case DEL: {
			break;
		}
		default: {
			break;
		}
		}
	}
}
char* id_name_gen() {
	static char id_name[idlen];
	sprintf(id_name, "$$%d", id_name_num++);
	return id_name;
}

char* label_name_gen() {
	static char label_name[idlen];
	sprintf(label_name,"Label%d", label_name_num++);
	return label_name;
}
char* numtostr(int num) {
	static char temp[idlen];
	sprintf(temp, "%d", num);
	return temp;
}
void delconst() {
	int i = 0;
	int j, k;
	while (i < midcode_loc) {
		for (; (midcode[i].type < 10 || midcode[i].type>12) && i < midcode_loc; i++);
		for (j = i + 1; j < midcode_loc; j++) {
			if (midcode[j].type == 0 || midcode[j].type == 1) {
				for (k = j + 1; k < midcode_loc; k++) {
					if (strcmp(midcode[k].argu1, midcode[j].argu1) == 0) {
						str_cpy(numtostr(midcode[j].value), midcode[k].argu1);
					}
					else if (strcmp(midcode[k].argu2, midcode[j].argu1) == 0) {
						str_cpy(numtostr(midcode[j].value), midcode[k].argu2);
					}
				}
			}
			if (midcode[j].type >= 10 && midcode[j].type <= 12)break;
		}
		i++;
	}

	/*
		替换全局常量，碰到局部常量或者变量就不替换
	*/
	int Func = 0;
	int index = 0;
	for (i = 0; midcode[i].type == 0 || midcode[i].type == 1; i++) {
		Func = 0;
		for (j = i + 1; j < midcode_loc; j++) {
			if (midcode[j].type >= 10 && midcode[j].type <= 10) {
				Func++;
			}
			if (strcmp(midcode[j].argu1, midcode[i].argu1) == 0) {
				index = searchtab(midcode[j].argu1, Func);
				if (tab[index].lev == 0) {
					str_cpy(numtostr(midcode[i].value), midcode[j].argu1);
				}
			}
			else if (strcmp(midcode[j].argu2, midcode[i].argu1) == 0) {
				index = searchtab(midcode[j].argu2, Func);
				if (tab[index].lev == 0) {
					str_cpy(numtostr(midcode[i].value), midcode[j].argu2);
				}
			}
		}
	}
}

