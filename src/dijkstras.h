#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int dst;
    int weight;
    Edge(int d, int w) : dst(d), weight(w) {}
};

struct Graph {
    int numVertices;
    vector<vector<Edge>> adjacencyList;
    
    Graph() : numVertices(0) {}
    
    void resize(int n) {
        numVertices = n;
        adjacencyList.resize(n);
    }
    
    const vector<Edge>& operator[](int index) const {
        return adjacencyList[index];
    }
    
    vector<Edge>& operator[](int index) {
        return adjacencyList[index];
    }
};

inline void file_to_graph(const string& filename, Graph& G) {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Cannot open graph file: " + filename);
    }
    
    int n, m;
    file >> n >> m;
    
    G.resize(n);
    
    for (int i = 0; i < m; ++i) {
        int src, dst, weight;
        file >> src >> dst >> weight;
        G[src].push_back(Edge(dst, weight));
    }
}

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous);
vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination);
void print_path(const vector<int>& path, int total);

#endif // DIJKSTRAS_H