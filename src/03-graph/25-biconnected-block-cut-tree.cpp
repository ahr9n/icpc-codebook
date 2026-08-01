/**
 * Biconnected components (2-vertex-connected blocks) + block-cut tree of an
 * undirected graph. One DFS with low-links and an edge stack: when a child v of
 * u satisfies low[v] >= num[u], every edge pushed since the tree edge (u,v) pops
 * off as one block, and u is a cut vertex (the root is a cut vertex only with
 * >= 2 DFS children). The block-cut tree has one node per block and one per cut
 * vertex, with an edge joining a cut vertex to each block that contains it; the
 * blocks alone partition the edges. run() fills bccs (edge-id lists) and tree
 * (adjacency: block ids 0..block_count-1, then a node per cut vertex). O(V+E)
 *
 * Contract: undirected, no self-loops; parallel edges are fine and land in the
 * same block. Isolated vertices belong to no block.
 */
struct BlockCutTree {
    int n, timer = 0, block_count = 0;
    vector<vector<pair<int, int>>> g;  // (neighbor, edge id)
    vector<pair<int, int>> edges;
    vector<int> low, num, edge_stack, cut_id;
    vector<bool> is_cut;
    vector<vector<int>> bccs, tree;

    BlockCutTree(int n) : n(n), g(n), low(n), num(n, -1), is_cut(n) {}

    void add_edge(int u, int v) {
        int id = edges.size();
        edges.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void dfs(int u, int parent_edge) {
        low[u] = num[u] = timer++;
        int children = 0;

        for (auto [v, id]: g[u]) {
            if (id == parent_edge) continue;
            if (num[v] != -1) {
                if (num[v] < num[u]) edge_stack.push_back(id);
                low[u] = min(low[u], num[v]);
                continue;
            }
            edge_stack.push_back(id);
            children++;
            dfs(v, id);
            low[u] = min(low[u], low[v]);

            if (low[v] >= num[u]) {
                if (parent_edge != -1) is_cut[u] = true;
                vector<int> block;
                while (true) {
                    int e = edge_stack.back();
                    edge_stack.pop_back();
                    block.push_back(e);
                    if (e == id) break;
                }
                bccs.push_back(block);
            }
        }

        if (parent_edge == -1 and children > 1) is_cut[u] = true;
    }

    void build_tree() {
        block_count = bccs.size();
        cut_id.assign(n, -1);
        int node = block_count;
        for (int u = 0; u < n; u++)
            if (is_cut[u]) cut_id[u] = node++;

        tree.assign(node, {});
        for (int b = 0; b < block_count; b++) {
            set<int> verts;
            for (int e: bccs[b]) {
                verts.insert(edges[e].first);
                verts.insert(edges[e].second);
            }
            for (int u: verts)
                if (is_cut[u]) {
                    tree[b].push_back(cut_id[u]);
                    tree[cut_id[u]].push_back(b);
                }
        }
    }

    void run() {
        for (int u = 0; u < n; u++)
            if (num[u] == -1) dfs(u, -1);
        build_tree();
    }
};

/**
 * Two triangles {0,1,2} and {2,3,4} sharing the cut vertex 2: two blocks, and a
 * block-cut tree of three nodes (block, block, cut vertex) with two edges.
 */
int main() {
    BlockCutTree bct(5);
    bct.add_edge(0, 1);
    bct.add_edge(1, 2);
    bct.add_edge(2, 0);
    bct.add_edge(2, 3);
    bct.add_edge(3, 4);
    bct.add_edge(4, 2);
    bct.run();

    int tree_edges = 0;
    for (auto& adj: bct.tree) tree_edges += adj.size();
    tree_edges /= 2;

    cout << bct.bccs.size() << " " << tree_edges << "\n";  // -> 2 2
    return 0;
}
