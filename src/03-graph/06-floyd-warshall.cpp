int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
vector<vector<long long>> dist_mat;

/**
 * All-pairs shortest paths. DP routing i->j through each k. dist_mat preset. O(V^3)
 */
void floyd_warshall() {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++) {
            long long dik = dist_mat[i][k];
            if (dik == LINF) continue;
            for (int j = 0; j < n; j++)
                if (dik + dist_mat[k][j] < dist_mat[i][j]) dist_mat[i][j] = dik + dist_mat[k][j];
        }
}

/**
 * Example: all-pairs shortest paths; check 0 -> 2 relaxed through 1.
 */
int main() {
    n = 3;
    dist_mat.assign(n, vector<long long>(n, LINF));
    for (int i = 0; i < n; i++) dist_mat[i][i] = 0;
    dist_mat[0][1] = 3;
    dist_mat[1][2] = 1;
    dist_mat[0][2] = 10;
    floyd_warshall();
    cout << dist_mat[0][2] << "\n";  // -> 4
    return 0;
}
