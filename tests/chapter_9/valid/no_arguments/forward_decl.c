int foo(void);

int main(void) {
    return !(foo() == 3);
}

int foo(void) {
    return 3;
}