#pragma once
struct FlowSolver {
    virtual ~FlowSolver() = default;
    virtual void init(int n) = 0;
    virtual void add_edge(int u, int v, long long cap, bool directed = true) = 0;
    virtual long long max_flow(int s, int t) = 0;
};