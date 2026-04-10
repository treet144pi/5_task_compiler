#pragma once

#include <vector>

class UnionFind {
    private:
        std::vector<int> p;

    public:
        explicit UnionFind(int n) : p(n) {
            for (int i = 0; i < n; ++i) {
                p[i] = i;
            }
        }

        int find(int x) {
            if (p[x] == x) {
                return x;
            }
            return p[x] = find(p[x]);
        }

        void merge_into(int x, int y) {
            x = find(x);
            y = find(y);
            if (x != y) {
                p[x] = y;
            }
        }
};
