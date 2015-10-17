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

int main() {
    int a,b,c;
    a = 5;
    b = 123;
    c = 15 + a + b;

    int z;
    z = a + b + c;
    z = fibonacci(11);
    printf("%d\n", z);
}
