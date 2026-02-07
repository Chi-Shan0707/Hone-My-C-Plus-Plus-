#pragma once
#include "flow_solver.hpp"
#include "dinic.hpp"

struct DinicWrapper : FlowSolver {
    Dinic<long long> impl;

    void init(int n) override { impl.init(n); }

    void add_edge(int u, int v, long long cap, bool directed = true) override {
        impl.add_edge(u, v, cap, directed);
    }

    long long max_flow(int s, int t) override { return impl.max_flow(s, t); }
};
