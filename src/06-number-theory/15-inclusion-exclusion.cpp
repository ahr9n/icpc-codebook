/**
 * Inclusion-exclusion over a set of pairwise-coprime divisors: how many integers
 * in [1, n] are divisible by at least one of them. Sum over every non-empty
 * subset S of (-1)^(|S|+1) * floor(n / product(S)); odd-sized subsets add, even
 * subsets subtract, cancelling the multiple-counting. The divisors must be
 * coprime so a subset's product is its lcm. O(2^k * k) for k divisors.
 */
long long count_divisible_by_any(long long n, vector<long long>& divisors) {
    int k = divisors.size();
    long long total = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        long long product = 1;
        int bits = 0;
        for (int i = 0; i < k; i++)
            if (mask & (1 << i)) product *= divisors[i], bits++;
        if (bits & 1)
            total += n / product;
        else
            total -= n / product;
    }
    return total;
}

/**
 * Example: in [1, 100], multiples of 2, 3, or 5 number 74, leaving 26 integers
 * coprime to 30.
 */
int main() {
    vector<long long> divisors = {2, 3, 5};
    cout << count_divisible_by_any(100, divisors) << "\n";  // -> 74
    return 0;
}
