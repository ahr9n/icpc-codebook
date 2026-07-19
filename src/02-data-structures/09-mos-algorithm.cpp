/**
 * MO's algorithm: answer Q offline range queries by sorting them into sqrt blocks,
 * then sliding [cur_l, cur_r] with cheap add/remove. O((N + Q) * sqrt(N))
 * Fill add/remove/get with the concrete per-query aggregate.
 */
struct MoQuery {
    int l, r, idx;
};

vector<long long> mo_solve(int arr_len, vector<MoQuery>& queries) {
    int block = max(1, (int)sqrt(arr_len));
    sort(queries.begin(), queries.end(), [&](MoQuery& a, MoQuery& b) {
        if (a.l / block != b.l / block) return a.l / block < b.l / block;
        return (a.l / block) & 1 ? a.r > b.r : a.r < b.r;
    });

    vector<long long> ans(queries.size());
    int cur_l = 0, cur_r = -1;
    auto add = [&](int i) {};
    auto remove = [&](int i) {};
    auto get = [&]() -> long long { return 0; };

    for (MoQuery& q: queries) {
        while (cur_r < q.r) add(++cur_r);
        while (cur_l > q.l) add(--cur_l);
        while (cur_r > q.r) remove(cur_r--);
        while (cur_l < q.l) remove(cur_l++);
        ans[q.idx] = get();
    }
    return ans;
}
