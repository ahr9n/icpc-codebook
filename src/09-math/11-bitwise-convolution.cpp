enum class BitwiseOp { Or, And, Xor };

/**
 * Walsh–Hadamard/zeta transforms compute OR, AND, or XOR convolution:
 * result[k] = sum a[i]*b[j] over pairs with (i op j) == k, modulo MOD.
 * Both input sizes must be the same power of two. Transforming both arrays,
 * multiplying pointwise, then applying the inverse takes O(n log n) time and
 * O(n) memory. The arrays must be non-empty and contain residues in [0, MOD);
 * MOD must be odd for the XOR inverse of 2 to exist.
 */
void bitwise_transform(vector<long long>& values, BitwiseOp op, bool inverse) {
    int n = values.size();
    long long inverse_two = (MOD + 1) / 2;

    for (int half = 1; half < n; half *= 2) {
        for (int start = 0; start < n; start += 2 * half) {
            for (int offset = 0; offset < half; offset++) {
                long long left = values[start + offset];
                long long right = values[start + offset + half];
                if (op == BitwiseOp::Or) {
                    if (inverse) {
                        values[start + offset + half] = (right - left + MOD) % MOD;
                    } else {
                        values[start + offset + half] = (right + left) % MOD;
                    }
                } else if (op == BitwiseOp::And) {
                    if (inverse) {
                        values[start + offset] = (left - right + MOD) % MOD;
                    } else {
                        values[start + offset] = (left + right) % MOD;
                    }
                } else {
                    values[start + offset] = (left + right) % MOD;
                    values[start + offset + half] = (left - right + MOD) % MOD;
                    if (inverse) {
                        values[start + offset] = values[start + offset] * inverse_two % MOD;
                        values[start + offset + half] =
                            values[start + offset + half] * inverse_two % MOD;
                    }
                }
            }
        }
    }
}

vector<long long> bitwise_convolution(vector<long long> a, vector<long long> b, BitwiseOp op) {
    bitwise_transform(a, op, false);
    bitwise_transform(b, op, false);
    for (int i = 0; i < (int)a.size(); i++) {
        a[i] = a[i] * b[i] % MOD;
    }
    bitwise_transform(a, op, true);
    return a;
}

/** Example: OR, AND, and XOR convolutions of the same two length-4 arrays. */
int main() {
    vector<long long> a = {1, 2, 3, 4};
    vector<long long> b = {4, 3, 2, 1};
    for (BitwiseOp op: {BitwiseOp::Or, BitwiseOp::And, BitwiseOp::Xor}) {
        vector<long long> result = bitwise_convolution(a, b, op);
        for (long long value: result) {
            cout << value << " ";
        }
        cout << "\n";
    }
    return 0;
}
// -> 4 17 20 59
// -> 59 20 17 4
// -> 20 22 28 30
