#pragma once
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Header-only Dinic max-flow (templated capacity type)
template<typename Cap = long long>
struct Dinic {
    struct Edge {
        int to;
        Cap cap;
        int rev; // index of reverse edge in g[to]
        Edge(int _to, Cap _cap, int _rev) : to(_to), cap(_cap), rev(_rev) {}
    };

    int N = 0;
    std::vector<std::vector<Edge>> g;
    std::vector<int> level, it;

    Dinic() = default;
    explicit Dinic(int n) { init(n); }

    void init(int n) {
        N = n;
        g.assign(N, {});
        level.assign(N, -1);
        it.assign(N, 0);
    }

    void add_edge(int u, int v, Cap cap, bool directed = true) {
        // u -> v with capacity cap; reverse edge capacity 0 (or cap if undirected)
        g[u].emplace_back(v, cap, (int)g[v].size());
        g[v].emplace_back(u, directed ? (Cap)0 : cap, (int)g[u].size() - 1);
    }

    bool bfs(int s, int t) {
        std::fill(level.begin(), level.end(), -1);
        std::queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const auto &e : g[u]) if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[u] + 1;
                if (e.to == t) return true;
                q.push(e.to);
            }
        }
        return level[t] >= 0;
    }

    Cap dfs(int u, int t, Cap f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)g[u].size(); ++i) {
            Edge &e = g[u][i];
            if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;
            Cap pushed = dfs(e.to, t, std::min(f, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;
                g[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
        return 0;
    }

    Cap max_flow(int s, int t) {
        const Cap INF = std::numeric_limits<Cap>::max();
        Cap flow = 0;
        while (bfs(s, t)) {
            std::fill(it.begin(), it.end(), 0);
            while (true) {
                Cap pushed = dfs(s, t, INF);
                if (pushed == 0) break;
                flow += pushed;
            }
        }
        return flow;
    }
};
