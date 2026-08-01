long long multiply_mod(long long a, long long b, long long mod) {
    return (long long)((__int128)a * b % mod);
}

long long power_mod(long long base, long long exponent, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) {
            result = multiply_mod(result, base, mod);
        }
        base = multiply_mod(base, base, mod);
        exponent >>= 1;
    }
    return result;
}

long long inverse_coprime(long long value, long long mod) {
    long long a = value;
    long long b = mod;
    __int128 x0 = 1;
    __int128 x1 = 0;
    while (b != 0) {
        long long quotient = a / b;
        tie(a, b) = make_pair(b, a % b);
        tie(x0, x1) = make_pair(x1, x0 - quotient * x1);
    }
    return (long long)((x0 % mod + mod) % mod);
}

long long coprime_discrete_log(long long base, long long target, long long mod) {
    long long block = sqrt((long double)mod);
    while ((__int128)block * block < mod) {
        block++;
    }
    while ((__int128)(block - 1) * (block - 1) >= mod) {
        block--;
    }
    unordered_map<long long, long long> baby;
    long long value = 1;
    for (long long exponent = 0; exponent < block; exponent++) {
        if (not baby.count(value)) {
            baby[value] = exponent;
        }
        value = multiply_mod(value, base, mod);
    }

    long long inverse_block = inverse_coprime(power_mod(base, block, mod), mod);
    value = target;
    for (long long giant = 0; giant < block; giant++) {
        auto it = baby.find(value);
        if (it != baby.end()) {
            __int128 exponent = (__int128)giant * block + it->second;
            if (exponent < mod) {
                return (long long)exponent;
            }
        }
        value = multiply_mod(value, inverse_block, mod);
    }
    return -1;
}

/**
 * Extended baby-step giant-step finds the smallest x >= 0 satisfying
 * base^x == target (mod mod), or -1 if none exists. Removing common gcd factors
 * first reduces the remaining instance to the usual meet-in-the-middle search
 * in O(sqrt(mod)) expected time and memory. Contract: mod >= 1 and base,target
 * are non-negative; __int128 keeps modular products exact for signed 64-bit
 * moduli. The O(sqrt(mod)) table must fit in memory.
 */
long long discrete_log(long long base, long long target, long long mod) {
    base %= mod;
    target %= mod;
    if (target == 1 % mod) {
        return 0;
    }

    long long removed = 0;
    long long prefix = 1 % mod;
    while (true) {
        long long common = gcd(base, mod);
        if (common == 1) {
            break;
        }
        if (target == prefix) {
            return removed;
        }
        if (target % common != 0) {
            return -1;
        }
        target /= common;
        mod /= common;
        prefix = multiply_mod(prefix, base / common, mod);
        removed++;
    }

    if (mod == 1) {
        return removed;
    }

    long long adjusted = multiply_mod(target, inverse_coprime(prefix, mod), mod);
    long long tail = coprime_discrete_log(base % mod, adjusted, mod);
    return tail == -1 ? -1 : removed + tail;
}

/**
 * Returns the smallest primitive root modulo prime p: a generator whose powers
 * visit every non-zero residue. Factoring p-1 and rejecting candidates that
 * collapse in any prime-factor subgroup takes O(sqrt(p) + g*k*log p) time,
 * where g is the returned root and k is the number of distinct factors of
 * p-1. Contract: p is prime; primitive_root(2) is 1.
 */
long long primitive_root(long long p) {
    if (p == 2) {
        return 1;
    }

    long long phi = p - 1;
    long long remaining = phi;
    vector<long long> prime_factors;
    for (long long divisor = 2; divisor <= remaining / divisor; divisor++) {
        if (remaining % divisor != 0) {
            continue;
        }
        prime_factors.push_back(divisor);
        while (remaining % divisor == 0) {
            remaining /= divisor;
        }
    }
    if (remaining > 1) {
        prime_factors.push_back(remaining);
    }

    for (long long candidate = 2; candidate < p; candidate++) {
        bool generates = true;
        for (long long factor: prime_factors) {
            if (power_mod(candidate, phi / factor, p) == 1) {
                generates = false;
                break;
            }
        }
        if (generates) {
            return candidate;
        }
    }
    return -1;
}

/** Example: two discrete logs and the smallest primitive root modulo 17. */
int main() {
    cout << discrete_log(2, 5, 13) << "\n";
    cout << discrete_log(4, 8, 14) << "\n";
    cout << primitive_root(17) << "\n";
    return 0;
}
// -> 9
// -> 3
// -> 3
