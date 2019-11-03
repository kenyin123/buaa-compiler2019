#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include"global.h"
/*
struct str_tab {
	char id[idlen];
	int type;
	int value;
	int addr;
	int size;
	int lev;
};
*/

int type = 0;
int value = 0;
int addr = 0;
int lev = 0;
struct str_tab tab[tab_size];
int tab_loc = 0;//当前在tab中间的位置,即符号表长度

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
		"int_array" , "char_array" ,
		"return_int_func" ,"return_char_func" ,
		"void_func",
		"int_para","char_para" 
	};
	for (int i = 0; i < tab_loc; i++) {
		fprintf(tab_out, "%s %s %d\n", type[tab[i].type], tab[i].id, tab[i].lev);
	}
}