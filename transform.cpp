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

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    struct photo {
        bool vert = false;
        vector<int> tags;
    };
    int n;
    cin >> n;
    map<string, int> m;
    int cnt = 0;
    vector<photo> photos(n);
    for (auto& p : photos) {
        char c;
        cin >> c;
        int k;
        cin >> k;
        p.vert = c == 'V';
        while (k--) {
            string s;
            cin >> s;
            if (m.find(s) == m.end()) m[s] = cnt++;
            p.tags.push_back(m[s]);
        }
    }
    cout << n << '\n';
    for (auto& [vert, tags] : photos) {
        cout << (vert ? 'V' : 'H') << ' ';
        cout << tags.size() << ' ';
        for (auto v : tags) cout << v << ' ';
        cout << '\n';
    }
}

