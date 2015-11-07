void printf();

int zero() {
    return 0;
}

void expect_equals(int actual_value, int expected_value, char* message) {
    char* red, green, reset;
    red = "\x1b[31m";
    green = "\x1b[32m";
    reset = "\x1b[0m";

    if (expected_value != actual_value) {
        printf("%s%s failed: expected %d but got %d%s\n", red, message, expected_value, actual_value, reset);
        return;
    } 
    printf("%s%s passed%s\n", green, message, reset);
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

struct test_struct {
    int v1;
    int v2;
};

int main() {
    struct test_struct t;
    struct test_struct* p;
    t.v1 = 10;
    t.v2 = 23;
    p = &t;

    expect_equals(t.v1, 10, "Struct member test 1");
    expect_equals(t.v2, 23, "Struct member test 2");
    expect_equals(*(p).v1, 10, "Struct member test 3");
    expect_equals(*(p).v2, 23, "Struct member test 4");

    expect_equals(1 + 1, 2, "Addition test 1");
    expect_equals(0 + 1, 1, "Addition test 2");
    expect_equals(-1 + 1, 0, "Addition test 3");

    expect_equals(1 - 1, 0, "Subtraction test 1");
    expect_equals(2 - 1, 1, "Subtraction test 2");

    expect_equals(1 * 1, 1, "Multiplication test 1");
    expect_equals(1 * 0, 0, "Multiplication test 2");
    expect_equals(3 * 5, 15, "Multiplication test 3");
    expect_equals(-5 * 5, -25, "Multiplication test 4");

    expect_equals(1 / 1, 1, "Division test 1");
    expect_equals(0 / 1, 0, "Divison test 2");
    expect_equals(25 / 5, 5, "Divison test 3");
    expect_equals(21 / 5, 4, "Divison test 4");

    expect_equals(2 % 1, 0, "Mod test 1");
    expect_equals(2 % 2, 0, "Mod test 2");
    expect_equals(3 % 2, 1, "Mod test 3");
    expect_equals(21 % 5, 1, "Mod test 4");

    expect_equals(fib(0), 0, "Fibonnaci test 1");    
    expect_equals(fib(1), 1, "Fibonnaci test 2");    
    expect_equals(fib(10), 55, "Fibonnaci test 3");    

    expect_equals(sum(0), 0, "Sum test 1");
    expect_equals(sum(1), 1, "Sum test 2");
    expect_equals(sum(10), 55, "Sum test 3");

    expect_equals(zero(), 0, "Function call test 1");

    return 0;
}
