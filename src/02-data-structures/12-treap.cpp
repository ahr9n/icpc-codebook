/**
 * Balanced BST via treap (tree + heap) with split/merge, augmented with subtree
 * sizes for order statistics. Randomized priorities keep it balanced in
 * expectation, so every operation is O(log n) expected. Supports duplicates as a
 * multiset. Split cuts on keys < key vs keys >= key, so each node's left subtree
 * is strictly smaller and its right subtree holds the equal keys.
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
        if (node == nullptr) return 0;
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
        if (left == nullptr) return right;
        if (right == nullptr) return left;
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
        if (node == nullptr) return nullptr;
        if (node->key == key) {
            removed = true;
            Node* merged = merge(node->left, node->right);
            delete node;
            return merged;
        }
        if (key < node->key)
            node->left = erase_one(node->left, key, removed);
        else
            node->right = erase_one(node->right, key, removed);
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

/**
 * Example: a multiset with a duplicate key, then an order-statistics query and a
 * rank query, before and after erasing one occurrence.
 */
int main() {
    Treap treap;
    for (long long key: {5, 3, 8, 1, 9, 3}) treap.insert(key);

    cout << treap.size() << "\n";           // -> 6
    cout << treap.kth(3) << "\n";           // -> 5
    cout << treap.order_of_key(8) << "\n";  // -> 4

    treap.erase(3);
    cout << treap.size() << "\n";           // -> 5
    cout << treap.order_of_key(8) << "\n";  // -> 3

    return 0;
}
