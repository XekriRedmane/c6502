/* Test initializing and updating a long long global variable */
static long long foo = 2147483640ll;

int main(void)
{
    if (foo + 5ll == 2147483645ll)
    {
        // assign a constant that can't fit in 16 bits; tests assembly rewrite rule
        foo = 1073741824ll;
        if (foo == 1073741824ll)
            return 1;
    }
    return 0;
}