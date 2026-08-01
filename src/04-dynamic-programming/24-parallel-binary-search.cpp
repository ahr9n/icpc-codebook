/**
 * Parallel binary search finds the first update prefix that satisfies every
 * offline query. All queries share the same ordered updates, and each query's
 * predicate must stay true after it first becomes true. Instead of replaying a
 * prefix for each query, bucket queries by their current midpoint, reset once,
 * and sweep the updates once per binary-search round.
 *
 * reset() restores the state before all updates, apply_update(i) applies the
 * 0-indexed update i, and is_satisfied(q) tests query q against the current
 * prefix. The answer is a prefix length in [0, update_count]; update_count + 1
 * means the query is never satisfied. Counts must be non-negative, and the state
 * is reset before returning. If reset costs R, an update costs U, and a check costs C, complexity
 * is O((R + update_count*U + query_count*C) * log(update_count + 2)) time and O(update_count +
 * query_count) memory.
 */
template <class Reset, class ApplyUpdate, class IsSatisfied>
vector<int> parallel_binary_search(int update_count, int query_count, Reset reset,
                                   ApplyUpdate apply_update, IsSatisfied is_satisfied) {
    vector<int> low(query_count, -1);
    vector<int> high(query_count, update_count + 1);

    while (true) {
        vector<vector<int>> bucket(update_count + 1);
        bool pending = false;
        for (int query = 0; query < query_count; query++) {
            if (low[query] + 1 < high[query]) {
                int mid = (low[query] + high[query]) / 2;
                bucket[mid].push_back(query);
                pending = true;
            }
        }
        if (not pending) {
            break;
        }

        reset();
        for (int prefix = 0; prefix <= update_count; prefix++) {
            if (prefix > 0) {
                apply_update(prefix - 1);
            }
            for (int query: bucket[prefix]) {
                if (is_satisfied(query)) {
                    high[query] = prefix;
                } else {
                    low[query] = prefix;
                }
            }
        }
    }
    reset();
    return high;
}

/** Example: first prefix reaching four point-value thresholds. */
int main() {
    vector<pair<int, int>> updates = {{0, 4}, {1, 3}, {0, 2}, {1, 5}};
    vector<pair<int, int>> queries = {{0, 0}, {0, 6}, {1, 7}, {1, 20}};
    vector<int> value(2);

    auto reset = [&]() { fill(value.begin(), value.end(), 0); };
    auto apply_update = [&](int i) { value[updates[i].first] += updates[i].second; };
    auto is_satisfied = [&](int q) { return value[queries[q].first] >= queries[q].second; };

    vector<int> answer = parallel_binary_search((int)updates.size(), (int)queries.size(), reset,
                                                apply_update, is_satisfied);
    for (int i = 0; i < (int)answer.size(); i++) {
        if (i > 0) {
            cout << " ";
        }
        cout << answer[i];
    }
    cout << "\n";
    return 0;
}
// -> 0 3 4 5
