void printf();

int fib(int n) {
    if (n < 1) {
        return 0;
    } 
    if (n == 1) {
        return 1;
    }
    return fib(n-1) + fib(n-2);
}

int sum(int n) {
    int sum;
    sum = 0;
    while (n > 0) {
        sum = sum + n;   
        n = n - 1;
    }
    return sum;
}

int main() {
    int a;
    a = 11;
    printf("a = %d\n", a);
    printf("fib %d = %d\n", a, fib(a));
    printf("sum %d = %d\n", a, sum(a));
}
