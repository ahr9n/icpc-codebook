/**
 * Least common multiple (divide first to avoid overflow).
 */
long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}
