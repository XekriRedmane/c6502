long long add(int a, int b) {
    return (long long) a + (long long) b;
}

int main(void) {
    long long a = add(125, 125);
    /* Test returning a long long from a function call */
    if (a == 250ll) {
        return 1;
    }
    return 0;
}