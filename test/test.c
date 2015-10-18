void printf();

int fibonacci(int n) {
    if (n < 1) {
        return 0;
    } 
    if (n == 1) {
        return 1;
    }
    return fibonacci(n-1) + fibonacci(n-2);
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
    int a,b;
    a = 10;
    b = 10;
    printf("%d\n", sum(a));
    printf("%d\n", a);
}
