/**
 * Coordinate compression: map values to their sorted rank [0, distinct). O(n log n)
 * comp[i] = rank of a[i]; sorted holds the original values by rank.
 */
vector<int> coordinate_compress(vector<long long>& a) {
    vector<long long> sorted = a;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    vector<int> comp(a.size());
    for (int i = 0; i < (int)a.size(); i++)
        comp[i] = lower_bound(sorted.begin(), sorted.end(), a[i]) - sorted.begin();
    return comp;
}
