int hi = 42;
int what;

int plug_one() {
    return 10;
}

int main() {
    int x = 10;
    int y = x + hi + 10;
    what = x + y + 100;
    return what + plug_one();
}
