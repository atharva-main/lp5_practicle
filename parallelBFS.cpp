// parallel_bfs_dfs_openmp.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Graph represented as adjacency list
class Graph {
public:
    int V;
    vector<vector<int>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // because it's an undirected graph
    }

    // Parallel BFS
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS starting from vertex " << start << ": ";

        while (!q.empty()) {
            int q_size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < q_size; i++) {
                int u;
                #pragma omp critical
                {
                    u = q.front();
                    q.pop();
                }

                cout << u << " ";

                for (int j=0; j < adj[u].size(); j++) {
                    int v = adj[u][j];
                    if (!visited[v]) {
                        #pragma omp critical
                        {
                            if (!visited[v]) {
                                visited[v] = true;
                                q.push(v);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // Parallel DFS using stack
    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;

        s.push(start);

        cout << "Parallel DFS starting from vertex " << start << ": ";

        while (!s.empty()) {
            int u;
            #pragma omp critical
            {
                u = s.top();
                s.pop();
            }

            if (!visited[u]) {
                cout << u << " ";
                visited[u] = true;
            }

            // Parallelize pushing neighbors onto stack
            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (!visited[v]) {
                    #pragma omp critical
                    {
                        if (!visited[v]) {
                            s.push(v);
                        }
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    int V = 7; // Number of vertices
    Graph g(V);

    // Create a sample undirected graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    double start_time, end_time;

    // Parallel BFS
    start_time = omp_get_wtime();
    g.parallelBFS(0);
    end_time = omp_get_wtime();
    cout << "Parallel BFS Time: " << (end_time - start_time) << " seconds" << endl;

    cout << "------------------------------------" << endl;

    // Parallel DFS
    start_time = omp_get_wtime();
    g.parallelDFS(0);
    end_time = omp_get_wtime();
    cout << "Parallel DFS Time: " << (end_time - start_time) << " seconds" << endl;

    return 0;
}
