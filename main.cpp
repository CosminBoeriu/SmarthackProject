#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge
{
    int from, to, capacity, cost;
    Edge(int a, int b, int c, int cost){
        this->from = a;
        this->to = b;
        this->capacity = c;
        this->cost = cost;
    }
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
            for (int i = 0; i < adj[cur].size(); i++ ) {
                int next = adj[cur][i];
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
    vector<int> parent = vector<int>(n, 0);
    int new_flow;
    new_flow = bfs(s, t, parent);
    while (new_flow != 0) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            int intoarcere = 0;
            if (prev < 0) {
                intoarcere = 1;
                prev = -prev;
            }
            capacity[intoarcere][prev][cur] -= new_flow;
            capacity[1 - intoarcere * 2][cur][prev] += new_flow;
            cur = prev;
        }
        new_flow = bfs(s, t, parent);
    }
    return flow;
}

void shortest_paths(int n, int v0, vector<int>& d, vector<int>& p) {
    d.assign(n, INF);
    d[v0] = 0;
    vector<bool> inq(n, false);
    queue<int> q;
    q.push(v0);
    p.assign(n, -1);
    vector<vector<bool>> visited(n, vector<bool>(n, false));

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (int intoarcere = 0; intoarcere < 2; intoarcere++) {
            for (int v: adj[u]) {
                if (!visited[u][v] && capacity[intoarcere][u][v] > 0 && d[v] > d[u] + cost[intoarcere][u][v]) {
                    visited[u][v] = true;
                    d[v] = d[u] + cost[intoarcere][u][v];
                    p[v] = u;
                    if (!inq[v]) {
                        inq[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }
}

pair<int, int> min_cost_flow(int N, vector<Edge> &edges, int K, int s, int t) {
    adj.assign(N+1, vector<int>());
    cost = vector<vector<vector<int>>>(2);
    cost[0].assign(N, vector<int>(N, 0));
    cost[1].assign(N, vector<int>(N, 0));
    capacity[0].assign(N, vector<int>(N, 0));
    capacity[1].assign(N, vector<int>(N, 0));
    for (Edge e : edges) {
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
        cost[0][e.from][e.to] = e.cost;
        cost[1][e.to][e.from] = -e.cost;
        capacity[0][e.from][e.to] = e.capacity;
    }

    int flow = 0;
    int cost_rez = 0;
    vector<int> d, p;
    while (flow < K) {
        shortest_paths(N, s, d, p);
        if (d[t] == INF)
            break;

        // find max flow on that path
        int f = K - flow;
        int cur = t;
        while (cur != s) {
            if(p[cur] < 0){
                f = min(f, capacity[1][-p[cur]][cur]);
                cur = -p[cur];
            }
            else{
                f = min(f, capacity[0][p[cur]][cur]);
                cur = p[cur];
            }
        }

        // apply flow
        flow += f;
        cost_rez += f * d[t];
        cur = t;
        while (cur != s) {
            int prev = p[cur];
            int intoarcere = 0;
            if (prev < 0) {
                intoarcere = 1;
                prev = -prev;
            }
            capacity[intoarcere][prev][cur] -= f;
            capacity[1 - intoarcere * 2][cur][prev] += f;
            cur = prev;
        }
    }


    return {flow, cost_rez};
}

int main() {
    int n, m, i, a, b, c, cst, start, finish, nr_rafinarii, nr_tancuri, nr_clienti;
    cin>>n>>m>>start>>finish;

    cin>>nr_rafinarii>>nr_tancuri>>nr_clienti;
    vector<int>clone(nr_rafinarii+nr_tancuri+1);

    vector<Edge>edges;

    for(i = 1; i <= nr_rafinarii; i++){
        // Refinery storage
        int penalty_ref = 1000;
        cin>>a;
        clone[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, a, penalty_ref);
    }
    for(i = nr_rafinarii + 1; i <= nr_rafinarii + nr_tancuri; i++){
        int penalty_tank = 1000;
        cin>>a;
        clone[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, a, penalty_tank);
    }
    n += nr_rafinarii + nr_tancuri;
    adj = vector<vector<int>>(n+1);
    capacity = vector<vector<vector<int>>>(2);
    capacity[0] = vector<vector<int>>(n+1);
    capacity[1] = vector<vector<int>>(n+1);

    for(i = 0; i <= n; i++){
        capacity[0][i] = vector<int>(n+1, 0);
        capacity[1][i] = vector<int>(n+1, 0);
    }
    for (i = 0; i < m; i++){
        cin>>a>>b>>c>>cst;
        a = clone[a];
        Edge e(a, b, c, cst);
        edges.push_back(e);
    }
    m = int(edges.size());
    for (i = 0; i < m; i++){
        Edge e = edges[i];
        adj[e.from].push_back(e.to);
        capacity[0][e.from][e.to] = e.capacity;
    }
    int flow = maxflow(n+1, start, finish);
    pair<int, int>p = min_cost_flow(n+1, edges, flow, start, finish);
    cout<<"Flow"<<' '<<"Cost"<<'\n';
    cout<<p.first<<' '<<p.second;
    return 0;
}
