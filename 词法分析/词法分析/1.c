
const int const_int1 = 1;  
const int const_int2 = 2, const_int3 = 3, const_int0 = -0;  
const char const_char1 = 'a';  
const char const_char2 = 'b', const_char3 = 'c';  
int var_int1;  
int var_int2, var_int_array[10];  
char var_char1;  
char var_char2, var_char_array[10];  
void printGlobalConsts(){  
    printf(const_int1);  
    printf(const_int2);  
    printf(const_int3);  
    printf(const_int0);  
    printf(const_char1);  
    printf(const_char2);  
    printf(const_char3);  
    return;  
}  
  
void printGlobalVariables(){  
    printf(var_int1);  
    printf(var_int2);  
    return;  
}  
void printfGlobalArrays(){  
    const int length = 10;  
    int i;  
    i = 0;  
    while(i < length){  
        var_int_array[i] = i;  
        printf(var_int_array[i]);  
        i = i + 1;  
    }  
    return;  
}  
  
void main(){  
    int int1, int2;  
    char char1, char2;  
    int i;  
    int n;  
    n = int1 + 10;  
  
    scanf(int1);  
    scanf(int2, char1, char2);  
    printGlobalConsts();  
    printGlobalVariables();  
    printfGlobalArrays();  
      
} 
