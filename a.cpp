#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")
// #pragma GCC target("avx2,sse4.2,bmi,bmi2,popcnt,lzcnt")

#include "bits/stdc++.h"

#ifdef DEBUG
    #include "includes/debug/debug.hpp"
#else
    #define debug(...) 0
#endif

using ll = int64_t;
using ull = uint64_t;
using ld = long double;

using namespace std;
struct photo {
    bool vert = false;
    basic_string<int> tags;
    pair<int, int> indices;
    friend ostream& operator<<(ostream& os, const photo& p) {
        os << (p.vert ? 'V' : 'H') << ' ';
        for (auto x : p.tags) os << x << ' ';
        os << ", " << p.indices.first << ' ' << p.indices.second;
        return os;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    cin >> n;
    vector<photo> photos(n);
    for (auto& p : photos) {
        char c;
        cin >> c;
        int k;
        cin >> k;
        p.vert = c == 'V';
        while (k--) {
            int x;
            cin >> x;
            p.tags.push_back(x);
        }
        sort(begin(p.tags), end(p.tags));
        p.indices = {&p - &*begin(photos), -1};
    }

    /*
       - number of tags
       - average intersection size
       - average B\A size
       - average A\B size
       */

    auto score = [](const photo& a, const photo& b) -> ll {
        basic_string<int> t;
        set_intersection(a.tags.begin(), a.tags.end(), b.tags.begin(),
                         b.tags.end(), back_inserter(t));
        return min(
            {t.size(), a.tags.size() - t.size(), b.tags.size() - t.size()});
    };

    auto combine = [](const photo& a, const photo& b) -> photo {
        assert(a.vert && b.vert);
        photo x;
        x.vert = false;
        set_union(a.tags.begin(), a.tags.end(), b.tags.begin(), b.tags.end(),
                  back_inserter(x.tags));
        x.indices = {a.indices.first, b.indices.first};
        return x;
    };

    random_shuffle(begin(photos), end(photos));

    int c =
        count_if(begin(photos), end(photos), [](auto& x) { return x.vert; });
    if (c & 1) {
        for (int i = 0; i < n; ++i) {
            if (photos[i].vert) {
                photos.erase(photos.begin() + i);
                break;
            }
        }
        --n;
    }

    // vertical - even

    vector<photo> ans{photos[0]};

    vector<int> remaining(n - 1);
    iota(remaining.begin(), remaining.end(), 1);

    for (int i = 1; i < n; ++i) {
        if (ans.back().vert) {
            if (i == 1) {
                auto it = find_if(begin(photos) + 1, end(photos),
                                  [](auto& x) { return x.vert; });
                assert(it != end(photos));
                ans.pop_back();
                ans.push_back(combine(photos[0], *it));
                remaining.erase(
                    find(begin(remaining), end(remaining), it - begin(photos)));
            } else {
                int best = -1;
                int index = -1;
                for (auto j : remaining) {
                    if (!photos[j].vert) continue;
                    auto comb = combine(ans.back(), photos[j]);
                    auto s = score(ans[ans.size() - 2], comb);
                    if (s > best) {
                        index = j;
                        best = s;
                    }
                }
                ans.back() = combine(ans.back(), photos[index]);
                remaining.erase(find(begin(remaining), end(remaining), index));
            }
        } else {
            int best = -1;
            int index = -1;
            for (auto j : remaining) {
                auto s = score(ans.back(), photos[j]);
                if (s > best) {
                    best = s;
                    index = j;
                }
            }
            ans.push_back(photos[index]);
            remaining.erase(find(begin(remaining), end(remaining), index));
        }
    }

    cout << ans.size() << '\n';
    for (auto& p : ans) {
        cout << p.indices.first << ' ';
        if (p.indices.second != -1) cout << p.indices.second << ' ';
        cout << '\n';
    }

    auto score_sol = [&](const vector<photo>& v) {
        ll ans = 0;
        for (int i = 0; i + 1 < (int)v.size(); ++i)
            ans += score(v[i], v[i + 1]);
        return ans;
    };

    cerr << score_sol(ans) << '\n';
}

