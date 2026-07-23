/**
 * Bitmask toolkit: an int is a set of bits, idx from the right, 0-based.
 * Use 1LL << idx once idx reaches 31+, since a plain 1 << idx overflows a 32-bit int.
 * Builtins (append ll for long long, e.g. __builtin_popcountll):
 *   __builtin_popcount(x)  set-bit count       x & -x       value of lowest set bit
 *   __builtin_ctz(x)       lowest set-bit idx  x & (x - 1)  clear the lowest set bit
 *   __builtin_clz(x)       leading zeros; 31 - clz(x) = highest set-bit index
 * bitset<N>: fixed N bits, ~8x less memory than bool[], fast whole-word ops
 *   (b.set()/reset()/flip(), b[i], b.count(), b.any(), b <<= k, b & c, b.to_ullong()).
 */

//  1011 >> 2 = 0010,  & 1 = 0  ->  get_bit(0b1011, 2) = 0
bool get_bit(int mask, int idx) {
    return (mask >> idx) & 1;
}

//  1010 | (1<<0)=0001  ->  1011  ->  set_bit(0b1010, 0) = 0b1011
int set_bit(int mask, int idx) {
    return mask | (1 << idx);
}

//  1011 & ~(1<<0)=1110  ->  1010  ->  clear_bit(0b1011, 0) = 0b1010
int clear_bit(int mask, int idx) {
    return mask & ~(1 << idx);
}

//  1011 ^ (1<<2)=0100  ->  1111  ->  flip_bit(0b1011, 2) = 0b1111
int flip_bit(int mask, int idx) {
    return mask ^ (1 << idx);
}

//  1100 & -(1100)=...0100  ->  0100  ->  lowest_bit(0b1100) = 0b0100
int lowest_bit(int mask) {
    return mask & -mask;
}

//  1000 & 0111 = 0  -> true;   1010 & 1001 = 1000 != 0  -> false
bool is_power_of_two(int mask) {
    return mask and not(mask & (mask - 1));
}

//  i ^ (i>>1):  0,1,3,2,6,7,5,4 for i=0..7 (each neighbor differs in one bit)
int gray_code(int i) {
    return i ^ (i >> 1);
}

/**
 * Example: each op on 0b1010, then enumerate all subsets and one mask's submasks.
 */
int main() {
    int x = 0b1010;                      // = 10
    cout << get_bit(x, 1) << "\n";       // bit 1 is set        -> 1
    cout << set_bit(x, 0) << "\n";       // 1010 -> 1011        -> 11
    cout << clear_bit(x, 1) << "\n";     // 1010 -> 1000        -> 8
    cout << flip_bit(x, 2) << "\n";      // 1010 -> 1110        -> 14
    cout << lowest_bit(x) << "\n";       // 1010 -> 0010        -> 2
    cout << is_power_of_two(8) << "\n";  // 1000 has one bit    -> 1
    cout << gray_code(3) << "\n";        // 3 ^ 1 = 2           -> 2

    int n = 3;  // all 2^n = 8 subsets: 0..7
    for (int mask = 0; mask < (1 << n); mask++) cout << mask << " ";
    cout << "\n";

    int mask = 0b1011;  // submasks of 1011: 1011 1010 1001 1000 0011 ...
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) cout << sub << " ";
    cout << "\n";
    return 0;
}
