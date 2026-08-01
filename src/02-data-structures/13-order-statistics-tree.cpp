#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

/**
 * Order-statistics tree via the GNU pb_ds policy tree ("ordered_set"): a
 * red-black tree that answers two extra queries in O(log n):
 *   find_by_order(k) -> iterator to the k-th smallest element (0-indexed)
 *   order_of_key(x)  -> number of elements strictly less than x
 *
 * WHY this is a separate listing: <ext/pb_ds/...> is a GNU libstdc++
 * extension. It compiles on GCC (Codeforces, most judges) but NOT on
 * clang/MSVC, so it can't share the portable template. When you need this on
 * a non-GNU toolchain, use the hand-written balanced BST in 12-treap.cpp,
 * which exposes the same two operations without the extension.
 *
 * The key type is unique like std::set. To act as a multiset (allow
 * duplicates) key on a pair<value, unique_tag> — e.g. insert {value, timer++}
 * so equal values stay distinct — then query order_of_key({x, 0}).
 */
using OrderedSet = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;

/** Example: k-th elements and strict ranks in a five-element ordered set. */
int main() {
    OrderedSet ordered;
    for (int x: {5, 1, 3, 8, 2}) {
        ordered.insert(x);
    }
    cout << *ordered.find_by_order(0) << '\n';
    cout << *ordered.find_by_order(2) << '\n';
    cout << *ordered.find_by_order(4) << '\n';

    cout << ordered.order_of_key(5) << '\n';
    cout << ordered.order_of_key(4) << '\n';
    cout << ordered.order_of_key(1) << '\n';
    cout << ordered.order_of_key(100) << '\n';

    return 0;
}
// -> 1
// -> 3
// -> 8
// -> 3
// -> 3
// -> 0
// -> 5
