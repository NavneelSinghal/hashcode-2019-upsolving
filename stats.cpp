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

    int max_tag = 0;
    int cnt_h = 0;
    int cnt_v = 0;
    int total_tags = 0;
    int max_tags = 0;
    int min_tags = 10000;

    for (auto& p : photos) {
        char c;
        cin >> c;
        int k;
        cin >> k;
        p.vert = c == 'V';
        if (p.vert)
            cnt_v++;
        else
            cnt_h++;
        total_tags += k;
        max_tags = max(max_tags, k);
        min_tags = min(min_tags, k);
        while (k--) {
            int x;
            cin >> x;
            p.tags.push_back(x);
            max_tag = max(max_tag, x);
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

    cerr << photos.size() << '\n';
    cerr << max_tag + 1 << '\n';
    cerr << cnt_h << '\n';
    cerr << cnt_v << '\n';
    cerr << (double)total_tags / photos.size() << '\n';
    cerr << max_tags << '\n';
    cerr << min_tags << '\n';

    auto score = [](const photo& a, const photo& b) -> ll {
        basic_string<int> t;
        set_intersection(a.tags.begin(), a.tags.end(), b.tags.begin(),
                         b.tags.end(), back_inserter(t));
        return min(
            {t.size(), a.tags.size() - t.size(), b.tags.size() - t.size()});
    };

    vector<vector<int>> photo_indices(n);

    ll total_size = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (score(photos[i], photos[j]) > 2) {
                photo_indices[i].push_back(j);
                photo_indices[j].push_back(i);
                total_size += 2;
            }
        }
    }

    if (total_size <= 1e6) {
        cerr << "sparse\n";
        for (int i = 0; i < n; ++i) {
            cout << i << '\n';
            cout << photo_indices[i].size() << '\n';
            for (auto j : photo_indices[i]) {
                cout << j << ' ';
            }
            cout << '\n';
        }
        cerr << total_size << '\n';
    }
}

