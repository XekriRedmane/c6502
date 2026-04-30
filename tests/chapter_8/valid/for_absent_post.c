int main(void) {
    int a = -123;
    for (; a % 5 != 0;) {
        a = a + 1;
    }
    return a % 5 || a > 0;
}
