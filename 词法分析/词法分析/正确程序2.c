
const int const_int1 = 1;  
const int const_int2 = 2, const_int3 = 3, const_int0 = -0;  
const char const_char1 = 'a';  
const char const_char2 = 'b', const_char3 = 'c';  
int var_int1;  
int var_int2, var_int_array[10];  
char var_char1;  
char var_char2, var_char_array[10];  
  
int pow(int a, int b){  
    int c;  
    if(b < 0) return (-1);  
    if(a == 0) return (0);  
    if(b == 0) return (1);  
    c = a + 0;  
    while(b > 1){  
        c = c * a;  
        b = b - 1;  
    }  
    return (c);  
}  
void testExpressions(){  
    const int const_int1 = -1;  
    const int const_int4 = 4;  
    const char const_char1 = 'A';  
    printf(const_char1);  
    printf(+const_char1);  
    printf(-const_char2);  
    printf(var_int_array[const_int0 + 1] + 'D');  
    printf(const_char1 * (var_char_array[0] + const_char2));  
    printf(-const_int1-(const_char2-'a')*(pow(const_int3, 2)*(const_int2 - const_int1)));  
}  
  
void main(){  
    int i;  
    i = 0;  
    while(i < 10){  
        var_int_array[i] = i;  
        i = i + 1;  
    }  
    testExpressions();  
  
}  
