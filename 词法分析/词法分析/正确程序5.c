
int x;  
  
int add(int a, int b, int c) {  
    x = x + a + b + c;  
    return (x);  
}  
  
void main() {  
    x = 1;  
    printf(x + add(add(add(1, 2, 3), add(0, 0, 0), 0), add(-3, x, -1), 0));  
}  
