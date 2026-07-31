/**
 * Probability (distribution) DP: carry the full probability distribution of a
 * running quantity and advance it one independent step at a time. Here the
 * quantity is the sum of dice_count fair dice, each with faces sides; adding a
 * die convolves the current distribution with the uniform 1..faces step.
 *
 *   dist[s] = P(sum == s),  reachable sums span [dice_count .. dice_count*faces]
 *
 * O(dice_count * max_sum * faces) time, O(max_sum) space.
 * Contract: the returned vector is indexed by the sum itself and its entries
 * sum to 1.
 */
vector<double> dice_sum_distribution(int dice_count, int faces) {
    int max_sum = dice_count * faces;
    vector<double> dist(max_sum + 1, 0.0);
    dist[0] = 1.0;
    double face_prob = 1.0 / faces;

    for (int d = 0; d < dice_count; d++) {
        vector<double> next(max_sum + 1, 0.0);
        for (int s = d; s <= d * faces; s++) {
            if (dist[s] == 0.0) continue;
            for (int f = 1; f <= faces; f++) next[s + f] += dist[s] * face_prob;
        }
        dist = next;
    }
    return dist;
}

int main() {
    cout << fixed << setprecision(4);

    vector<double> dist = dice_sum_distribution(2, 6);
    cout << "P(sum=7)  with 2d6 = " << dist[7] << "\n";
    cout << "P(sum=2)  with 2d6 = " << dist[2] << "\n";
    cout << "P(sum=12) with 2d6 = " << dist[12] << "\n";

    int best_sum = 0;
    for (int s = 0; s < (int)dist.size(); s++)
        if (dist[s] > dist[best_sum]) best_sum = s;
    cout << "most likely sum   = " << best_sum << "\n";
    return 0;
}
// -> P(sum=7)  with 2d6 = 0.1667
// -> P(sum=2)  with 2d6 = 0.0278
// -> P(sum=12) with 2d6 = 0.0278
// -> most likely sum   = 7
