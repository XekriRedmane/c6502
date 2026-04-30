int main(void) {
    /* initializing a tests the rewrite rule for
     * movq $large_const, memory_address
     */
    long long a = 2147483640ll;
    long long b = 0ll;
    /* Assign the value of one long long variable
     * (which is too large for an int or long to represent)
     * to another long long variable
     */
    b = a;
    return (b == 2147483640ll);
}