// Compare discrete logs with explicit power-orbit enumeration on small moduli,
// and verify primitive roots generate every non-zero residue of small primes.
int stress() {
    long long checks = 0;

    for (long long mod = 1; mod <= 160; mod++) {
        for (long long base = 0; base < mod; base++) {
            vector<long long> first(mod, -1);
            long long value = 1 % mod;
            for (long long exponent = 0; first[value] == -1; exponent++) {
                first[value] = exponent;
                value = multiply_mod(value, base, mod);
            }
            for (long long target = 0; target < mod; target++) {
                long long actual = discrete_log(base, target, mod);
                if (actual != first[target]) {
                    printf("discrete-log FAIL mod=%lld base=%lld target=%lld expected=%lld "
                           "actual=%lld\n",
                           mod, base, target, first[target], actual);
                    return 1;
                }
                checks++;
            }
        }
    }

    const long long medium_prime = 1000003;
    long long medium_root = primitive_root(medium_prime);
    mt19937 rng(621);
    for (int trial = 0; trial < 2000; trial++) {
        long long exponent = rng() % (medium_prime - 1);
        long long target = power_mod(medium_root, exponent, medium_prime);
        long long actual = discrete_log(medium_root, target, medium_prime);
        if (actual != exponent) {
            printf("discrete-log FAIL medium exponent=%lld actual=%lld\n", exponent, actual);
            return 1;
        }
        checks++;
    }

    auto is_prime = [](long long value) {
        if (value < 2) {
            return false;
        }
        for (long long divisor = 2; divisor * divisor <= value; divisor++) {
            if (value % divisor == 0) {
                return false;
            }
        }
        return true;
    };

    for (long long p = 2; p <= 1000; p++) {
        if (not is_prime(p)) {
            continue;
        }
        long long root = primitive_root(p);
        vector<bool> seen(p, false);
        long long value = 1 % p;
        for (long long exponent = 0; exponent < p - 1; exponent++) {
            seen[value] = true;
            value = multiply_mod(value, root, p);
        }
        for (long long residue = 1; residue < p; residue++) {
            if (not seen[residue]) {
                printf("primitive-root FAIL p=%lld root=%lld residue=%lld\n", p, root, residue);
                return 1;
            }
            checks++;
        }
    }

    printf("discrete-log-primitive-root PASS %lld", checks);
    return 0;
}
