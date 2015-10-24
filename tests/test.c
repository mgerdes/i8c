void printf();

void expect(char *message, int b) {
    if (!b) {
        printf("%s failed\n", message);
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
    expect("Addition test 1", 1 + 1 == 2);
    expect("Addition test 2", 0 + 1 == 1);
    expect("Addition test 3", -1 + 1 == 0);

    expect("Subtration test 1", 1 - 1 == 0);
    expect("Subtration test 2", 2 - 1 == 1);

    expect("Fibonnacci test 1", fib(0) == 0);    
    expect("Fibonnacci test 2", fib(1) == 1);    
    expect("Fibonnacci test 3", fib(10) == 55);    

    expect("Sum test 1", sum(0) == 0);
    expect("Sum test 2", sum(1) == 1);
    expect("Sum test 3", sum(10) == 55);
}
