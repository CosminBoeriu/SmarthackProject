#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <queue>

using namespace std;

struct Edge
{
    int from, to, capacity, cost;
};

vector<vector<int>> adj;
vector<vector<vector<int>>> cost, capacity;

const int INF = 1e9;

// start node, end node,
int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), 0);
    //s nu e 0 va rog
    parent[s] = s;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();
        for (int intoarcere = 0; intoarcere < 2; intoarcere++) {
            for (int next = 0; next < adj[cur].size(); next++ ) {
                if (parent[next] == 0 && capacity[intoarcere][cur][next]) {
                    // If intoarcere = 0 => normal edge, else I took the augmenting edge
                    parent[next] = cur * (1 - intoarcere * 2);
                    int new_flow = min(flow, capacity[intoarcere][cur][next]);
                    if (next == t)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
    }
    return 0;
}

// Num of vertices, start node, final node - the added ones
int maxflow(int n, int s, int t) {
    int flow = 0;
    vector<int> parent(n);
    int new_flow;
    new_flow = bfs(s, t, parent);
    while (new_flow != 0) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            int intoarcere = 0;
            if (prev < 0)
                intoarcere = 1;
            capacity[intoarcere][prev][cur] -= new_flow;
            capacity[1 - intoarcere * 2][prev][cur] += new_flow;
            cur = prev;
        }
        new_flow = bfs(s, t, parent);
    }
    return flow;
}
/*
void shortest_paths(int n, int v0, vector<int>& d, vector<int>& p) {
    d.assign(n, INF);
    d[v0] = 0;
    vector<bool> inq(n, false);
    queue<int> q;
    q.push(v0);
    p.assign(n, -1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (int v : adj[u]) {
            if (capacity[u][v] > 0 && d[v] > d[u] + cost[u][v]) {
                d[v] = d[u] + cost[u][v];
                p[v] = u;
                if (!inq[v]) {
                    inq[v] = true;
                    q.push(v);
                }
            }
        }
    }
}

int min_cost_flow(int N, vector<Edge> edges, int K, int s, int t) {
    adj.assign(N, vector<int>());
    cost.assign(N, vector<int>(N, 0));
    capacity.assign(N, vector<int>(N, 0));
    for (Edge e : edges) {
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
        cost[e.from][e.to] = e.cost;
        cost[e.to][e.from] = -e.cost;
        capacity[e.from][e.to] = e.capacity;
    }

    int flow = 0;
    int cost = 0;
    vector<int> d, p;
    while (flow < K) {
        shortest_paths(N, s, d, p);
        if (d[t] == INF)
            break;

        // find max flow on that path
        int f = K - flow;
        int cur = t;
        while (cur != s) {
            f = min(f, capacity[p[cur]][cur]);
            cur = p[cur];
        }

        // apply flow
        flow += f;
        cost += f * d[t];
        cur = t;
        while (cur != s) {
            capacity[p[cur]][cur] -= f;
            capacity[cur][p[cur]] += f;
            cur = p[cur];
        }
    }

    if (flow < K)
        return -1;
    else
        return cost;
}
*/
int main() {
    std::cout << "Hello, World!" << std::endl;
    adj = vector<vector<int>>(2);
    adj[1] = {2, 3};
    adj[2] = {3, 4};
    adj[3] = {2};
    adj[4] = {};
    capacity[0] =

    return 0;
}
