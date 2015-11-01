void printf();

int zero() {
    return 0;
}

void expect(char *message, int b) {
    char* red, green, reset;
    red = "\x1b[31m";
    green = "\x1b[32m";
    reset = "\x1b[0m";

    if (!b) {
        printf("%s %sfailed%s\n", message, red, reset);
        return;
    } 
    printf("%s %spassed%s\n", message, green, reset);
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
    for (sum = 0; n > 0; n = n - 1) {
        sum = sum + n;
    }
    return sum;
}

struct test_stuct {
    int v1;
    int v2;
};

int main() {
    struct test_stuct t;
    t.v1 = 10;
    t.v2 = 23;
    expect("Struct member test 1", t.v1 == 10);
    expect("Struct member test 2", t.v2 == 23);

    expect("Addition test 1", 1 + 1 == 2);
    expect("Addition test 2", 0 + 1 == 1);
    expect("Addition test 3", -1 + 1 == 0);

    expect("Subtration test 1", 1 - 1 == 0);
    expect("Subtration test 2", 2 - 1 == 1);

    expect("Multiplication test 1", 1 * 1 == 1);
    expect("Multiplication test 2", 1 * 0 == 0);
    expect("Multiplication test 3", 3 * 5 == 15);
    expect("Multiplication test 4", -5 * 5 == -25);

    expect("Division test 1", 1 / 1 == 1);
    expect("Division test 2", 0 / 1 == 0);
    expect("Division test 3", 25 / 5 == 5);
    expect("Division test 4", 21 / 5 == 4);

    expect("Mod test 1", 2 % 1 == 0);
    expect("Mod test 2", 2 % 2 == 0);
    expect("Mod test 3", 3 % 2 == 1);
    expect("Mod test 3", 21 % 5 == 1);

    expect("Not equal test 1", 2 != 0);
    expect("Not equal test 2", 2 != 1);

    expect("Fibonnacci test 1", fib(0) == 0);    
    expect("Fibonnacci test 2", fib(1) == 1);    
    expect("Fibonnacci test 3", fib(10) == 55);    

    expect("Sum test 1", sum(0) == 0);
    expect("Sum test 2", sum(1) == 1);
    expect("Sum test 3", sum(10) == 55);

    expect("Function call test 1", zero() == 0);

    return 0;
}
