#include <bits/stdc++.h>
#include "flow_solver.hpp"
#include "dinic_wrapper.hpp"
using namespace std;

int main() {
    // Example: simple graph with max flow = 5
    unique_ptr<FlowSolver> solver = make_unique<DinicWrapper>();
    int n = 6; // nodes 0..5
    int s = 0, t = 5;
    solver->init(n);
    // Edges: 0->1(10), 0->2(5), 1->2(15), 1->3(10), 2->4(10), 3->4(10), 3->5(10), 4->5(10)
    solver->add_edge(0, 1, 10);
    solver->add_edge(0, 2, 5);
    solver->add_edge(1, 2, 15);
    solver->add_edge(1, 3, 10);
    solver->add_edge(2, 4, 10);
    solver->add_edge(3, 4, 10);
    solver->add_edge(3, 5, 10);
    solver->add_edge(4, 5, 10);

    long long f = solver->max_flow(s, t);
    cout << f << "\n"; // prints max flow
    return 0;
}
