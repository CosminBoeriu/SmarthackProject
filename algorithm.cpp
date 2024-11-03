#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include "parsers.cpp"

using namespace std;

vector<vector<string>>mapper_connection_ids = vector<vector<string>>(1000, vector<string>(1000, ""));
vector<int>cloned;
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

vector<vector<vector<int>>> min_cost_flow(int N, vector<Edge> &edges, int K, int s, int t) {
    adj.assign(N+1, vector<int>());
    vector<vector<vector<int>>>rez;
    cost = vector<vector<vector<int>>>(2);
    cost[0].assign(N, vector<int>(N, 0));
    cost[1].assign(N, vector<int>(N, 0));
    capacity[0].assign(N, vector<int>(N, 0));
    capacity[1].assign(N, vector<int>(N, 0));
    for (Edge e : edges) {
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
        capacity[0][e.from][e.to] = e.capacity;
    }

    int flow = 0;
    int cost_rez = 0;
    vector<int> d, p;
    int index = 0;
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
        rez.emplace_back();
        cur = t;
        while (cur != s) {
            int prev = p[cur];
            int intoarcere = 0;
            if (prev < 0) {
                intoarcere = 1;
                prev = -prev;
                rez[index].emplace_back(vector<int>{cur, prev, -f});
            }
            else{
                rez[index].emplace_back(vector<int>{prev, cur, f});
            }
            capacity[intoarcere][prev][cur] -= f;
            capacity[1 - intoarcere * 2][cur][prev] += f;

            cur = prev;
        }
    }

    return rez;
}

vector<Edge>edges;
int n, m, start = 1, finish, nr_rafinarii, nr_tancuri, nr_clienti;
vector<vector<vector<int>>> init_network(vector<Refinery>&ref, vector<Customer>&cust, vector<Tank>& tnks, vector<Connection>&conns, map<string, int>&mp){
    vector<Edge>v;
    int i, a;
    nr_rafinarii = ref.size();
    nr_tancuri = tnks.size();
    nr_clienti = cust.size();

    n = nr_rafinarii + nr_tancuri + nr_clienti;
    m = conns.size();

    for(Connection conn: conns){
        //from, to, cap, cost;
        if(conn.connection_type.compare("TRUCK") == 1)
            conn.distance = conn.distance * 10;
        v.emplace_back(mp[conn.from_id], mp[conn.to_id], conn.max_capacity, conn.distance );
        mapper_connection_ids[mp[conn.from_id]][mp[conn.to_id]] = conn.id;
    }
    cloned = vector<int>(nr_rafinarii+nr_tancuri+1, 0);

    for(i = 2; i < nr_rafinarii + 2; i++){
        edges.emplace_back(1, 2, INF, 0);
    }
    for(i = nr_rafinarii + nr_tancuri + 2; i <= nr_rafinarii + nr_tancuri + nr_clienti + 1; i++){
        edges.emplace_back(i, nr_rafinarii + nr_tancuri + nr_clienti + 2, 0, 0);
    }
    n += 2;
    for(i = 2; i <= nr_rafinarii + 1; i++){
        // Refinery storage
        int penalty_ref = refineries[i].overflow_penalty * 100;
        a = refineries[i].initial_stock;
        cloned[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, refineries[i].capacity, penalty_ref);
    }
    for(i = nr_rafinarii + 2; i <= nr_rafinarii + nr_tancuri + 1; i++){
        int penalty_tank = tanks[i].overflow_penalty * 100;
        a = tanks[i].initial_stock;
        cloned[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, tanks[i].initial_stock, penalty_tank);
    }
    n += nr_rafinarii + nr_tancuri;

    cost = vector<vector<vector<int>>>(2);
    cost[0].assign(n, vector<int>(n, 0));
    cost[1].assign(n, vector<int>(n, 0));
    capacity[0].assign(n, vector<int>(n, 0));
    capacity[1].assign(n, vector<int>(n, 0));
    for(i = 0; i <= n; i++){
        capacity[0][i] = vector<int>(n+1, 0);
        capacity[1][i] = vector<int>(n+1, 0);
    }
    for (Edge e : edges) {
        adj[e.from].push_back(e.to);
        adj[e.to].push_back(e.from);
        cost[0][e.from][e.to] = e.cost;
        cost[1][e.to][e.from] = -e.cost;
        capacity[0][e.from][e.to] = e.capacity;
    }
    /*
    m = int(edges.size());
    for (i = 0; i < m; i++){
        Edge e = edges[i];
        adj[e.from].push_back(e.to);
        capacity[0][e.from][e.to] = e.capacity;
    }
    */
    return capacity;
}

void update_capacity_matrix(vector<vector<int>>mat){
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            capacity[0][i][j] = mat[i][j];
        }
    }
}

vector<vector<vector<int>>> algorithm() {
    int i, a, b, c, cst;
    /*
    vector<int>clone(nr_rafinarii+nr_tancuri+1);

    for(i = 2; i < nr_rafinarii + 2; i++){
        edges.emplace_back(1, 2, INF, 0);
    }
    for(i = nr_rafinarii + nr_tancuri + 2; i <= nr_rafinarii + nr_tancuri + nr_clienti + 1; i++){
        edges.emplace_back(i, nr_rafinarii + nr_tancuri + nr_clienti + 2, 0, 0);
    }
    n += 2;
    for(i = 2; i <= nr_rafinarii + 1; i++){
        // Refinery storage
        int penalty_ref = refineries[i].overflow_penalty * 100;
        a = refineries[i].initial_stock;
        clone[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, refineries[i].capacity, penalty_ref);
    }
    for(i = nr_rafinarii + 2; i <= nr_rafinarii + nr_tancuri + 1; i++){
        int penalty_tank = tanks[i].overflow_penalty * 100;
        a = tanks[i].initial_stock;
        clone[i] = n+i;
        edges.emplace_back(i, n+i, a, 0);
        edges.emplace_back(n+i, i, tanks[i].initial_stock, penalty_tank);
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
    */
    int flow = maxflow(n+1, start, finish);
    return min_cost_flow(n+1, edges, flow, start, finish);
}
