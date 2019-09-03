
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
  
  
void testIf(int a, int b){  
    printf(a);  
    printf(b);  
    if(a <= b){  
        printf("a is smaller than b");  
        a = a + b;  
        printf("total:",a);  
    }  
}  
  
void testWhile(){  
    int i, j;  
    i = 0;  
    j = 2;  
    while(j < i){  
        printf("j is smaller than i, j is:",j);  
        j = j + 1;  
    }  
    while(i < j){  
        printf("i is smaller than j, i is:", i);  
        i = i + 1;  
    }  
}  
  
void testSwitch(char a, char b){  
    int c;  
    c = a - b;  
    switch(a-b){  
        case -1:  
            printf("hello");  
        case 1:  
            printf("world");  
    }  
    switch(a){  
        case 'a':  
            printf("lucky, a is 'a'");  
        case 'b':  
            printf("lucky, a is 'b'");  
        default:{  
            printf("c=",c);  
            if(c >= 0)  
                while(c >- 1){  
                   printf("c=",c);   
                   c = c - 1;  
                }   
            printf("I can't see.");  
        }  
    }  
}  
  
  
  
void main(){  
    int i;  
    i = 0;  
    while(i < 10){  
        var_int_array[i] = i;  
        i = i + 1;  
    }  
      
  
    testIf(1, 2);  
  
    testWhile();  
  
    testSwitch('a', 'b');  
    testSwitch('c', 'a');  
  
}
