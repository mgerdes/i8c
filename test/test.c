float hi = 42.2;
int what;

int plug_one() {
    return 10;
}

int main() {
    float z;
    z = hi + 10;
    {
        int x;
    }
    int x = 10;
    int y = x + 10;
    what = x + y + 100;
    return what + plug_one();
}
