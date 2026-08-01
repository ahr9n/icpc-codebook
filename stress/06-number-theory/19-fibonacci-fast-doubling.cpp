// Stress: fast-doubling fib(n) mod MOD vs an iterative O(n) reference for all
// n up to a few hundred thousand, plus explicit small edge cases.
int stress() {
    if (fib(0) != 0 or fib(1) != 1 or fib(2) != 1) {
        printf("FAIL edge n=0,1,2\n");
        return 1;
    }
    const int limit = 300000;
    long long prev = 0, cur = 1;  // F(0), F(1)
    long long checks = 0;
    for (int n = 0; n <= limit; n++) {
        if (fib(n) != prev) {
            printf("FAIL n=%d got=%lld want=%lld\n", n, fib(n), prev);
            return 1;
        }
        checks++;
        long long next = (prev + cur) % MOD;
        prev = cur;
        cur = next;
    }
    printf("fibonacci PASS %lld", checks);
    return 0;
}
