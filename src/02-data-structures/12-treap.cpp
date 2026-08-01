/**
 * Balanced BST via treap (tree + heap) with split/merge, augmented with subtree
 * sizes for order statistics. Randomized priorities keep it balanced in
 * expectation, so every operation is O(log n) expected. Supports duplicates as a
 * multiset. Split cuts on keys < key vs keys >= key; after merging in a duplicate,
 * equal keys can be on either side while the in-order sequence stays sorted.
 *
 * kth(k) is 0-indexed and returns LINF when k is out of range.
 * order_of_key(x) counts elements strictly less than x.
 */
mt19937 treap_rng(1234567);

struct Treap {
    struct Node {
        long long key;
        unsigned priority;
        int subtree_size;
        Node *left, *right;

        Node(long long key)
            : key(key), priority(treap_rng()), subtree_size(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    int size_of(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->subtree_size;
    }

    void pull(Node* node) {
        node->subtree_size = 1 + size_of(node->left) + size_of(node->right);
    }

    void split(Node* node, long long key, Node*& left, Node*& right) {
        if (node == nullptr) {
            left = nullptr;
            right = nullptr;
            return;
        }
        if (node->key < key) {
            split(node->right, key, node->right, right);
            left = node;
        } else {
            split(node->left, key, left, node->left);
            right = node;
        }
        pull(node);
    }

    Node* merge(Node* left, Node* right) {
        if (left == nullptr) {
            return right;
        }
        if (right == nullptr) {
            return left;
        }
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            pull(left);
            return left;
        }
        right->left = merge(left, right->left);
        pull(right);
        return right;
    }

    void insert(long long key) {
        Node *left, *right;
        split(root, key, left, right);
        root = merge(merge(left, new Node(key)), right);
    }

    Node* erase_one(Node* node, long long key, bool& removed) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == key) {
            removed = true;
            Node* merged = merge(node->left, node->right);
            delete node;
            return merged;
        }
        if (key < node->key) {
            node->left = erase_one(node->left, key, removed);
        } else {
            node->right = erase_one(node->right, key, removed);
        }
        pull(node);
        return node;
    }

    bool erase(long long key) {
        bool removed = false;
        root = erase_one(root, key, removed);
        return removed;
    }

    long long kth(int k) {
        Node* node = root;
        while (node != nullptr) {
            int left_size = size_of(node->left);
            if (k < left_size) {
                node = node->left;
            } else if (k == left_size) {
                return node->key;
            } else {
                k -= left_size + 1;
                node = node->right;
            }
        }
        return LINF;
    }

    int order_of_key(long long key) {
        Node* node = root;
        int rank = 0;
        while (node != nullptr) {
            if (node->key < key) {
                rank += size_of(node->left) + 1;
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return rank;
    }

    int size() {
        return size_of(root);
    }
};

/** Example: multiset rank and k-th queries before and after one erase. */
int main() {
    Treap treap;
    for (long long key: {5, 3, 8, 1, 9, 3}) {
        treap.insert(key);
    }

    cout << treap.size() << "\n";
    cout << treap.kth(3) << "\n";
    cout << treap.order_of_key(8) << "\n";

    treap.erase(3);
    cout << treap.size() << "\n";
    cout << treap.order_of_key(8) << "\n";

    return 0;
}
// -> 6
// -> 5
// -> 4
// -> 5
// -> 3
