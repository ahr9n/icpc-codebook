// Compare all three transformed convolutions with their direct O(n^2)
// definitions on random arrays, including the one-element transform.
int stress() {
    mt19937 rng(911);
    long long checks = 0;

    for (int rep = 0; rep < 5000; rep++) {
        int bits = rng() % 6;
        int n = 1 << bits;
        vector<long long> a(n), b(n);
        for (int i = 0; i < n; i++) {
            a[i] = rng() % 1000;
            b[i] = rng() % 1000;
        }

        for (BitwiseOp op: {BitwiseOp::Or, BitwiseOp::And, BitwiseOp::Xor}) {
            vector<long long> expected(n, 0);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int index = 0;
                    if (op == BitwiseOp::Or) {
                        index = i | j;
                    } else if (op == BitwiseOp::And) {
                        index = i & j;
                    } else {
                        index = i ^ j;
                    }
                    expected[index] = (expected[index] + a[i] * b[j]) % MOD;
                }
            }

            vector<long long> actual = bitwise_convolution(a, b, op);
            if (actual != expected) {
                printf("bitwise-convolution FAIL rep=%d bits=%d op=%d\n", rep, bits, (int)op);
                return 1;
            }
            checks += n;
        }
    }

    printf("bitwise-convolution PASS %lld", checks);
    return 0;
}
