void printf();

void expect(int b) {
    if (!b) {
        printf("A test failed :(\n");
    }
}

void print_triangle(int n) {
    printf("%d\n", n);
    int a, i, j;
    a = 1;
    i = 1;
    while (i < n) {
        j = 1;
        while (j < i) {
            printf("%d ", a);
            a = a + 1;
            j = j + 1;
        }
        if (i > 1) {
            printf("\n");
        }
        i = i + 1;
    }
}

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
    int *b;

    expect(10 + 10 == 20);
    
    b = 0;
    printf("!b = %d\n", !b);
    b = 1;
    printf("!b = %d\n", !b);

    b = &a;
    a = 11;
    printf("b = %u\n", b);
    printf("a = %d\n", a);
    print_triangle(a);
    printf("fib %d = %d\n", a, fib(a));
    printf("sum %d = %d\n", a, sum(a));
}
