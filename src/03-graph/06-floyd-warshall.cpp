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
