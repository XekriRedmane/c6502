/* Test initializing and updating unsigned global variables */
static unsigned long long x = 2147483643ull; // 2^31 - 5

// make sure these are initialized to zero
unsigned long long zero_long;
unsigned zero_int;

int main(void)
{
    if (x != 2147483643ull)
        return 0;
    x = x + 10;
    if (x != 2147483653ull)
        return 0;
    if (zero_long || zero_int)
        return 0;
    return 1;
}