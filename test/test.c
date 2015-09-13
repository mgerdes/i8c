int hi = 42;

int plug_one() {
    return 10;
}

int main() {
    int array[10];
    int x = array + 10;
    int y = x + hi + 10;
    return x + y + plug_one();
}
