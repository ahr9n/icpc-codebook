#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;
const long long MOD = 1e9 + 7, LINF = 0x3f3f3f3f3f3f3f3fLL;
const long double PI = acosl(-1);

int n, m;

/**
 * Grid bounds check against globals n, m.
 */
bool is_valid(int i, int j) {
    return i >= 0 and j >= 0 and i < n and j < m;
}

void solve() {}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) solve();

    return 0;
}
