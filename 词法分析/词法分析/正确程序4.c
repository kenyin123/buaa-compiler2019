
int Fibonacci(int n){  
    if(n <= 0) return (0);  
    if(n == 1) return (1);  
    if(n == 2) return (1);  
    return (Fibonacci(n - 1) + Fibonacci(n - 2));  
}  
  
  
  
void main(){  
     printf("Fibonacci(0) = ", Fibonacci(0));  
    printf("Fibonacci(1) = ", Fibonacci(1));  
    printf("Fibonacci(10) = ", Fibonacci(10));  
}  
