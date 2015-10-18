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
}

int main() {
    int a,b;
    a = fibonacci(11);
    b = sum(10);
    printf("%d\n", a + b);
}
