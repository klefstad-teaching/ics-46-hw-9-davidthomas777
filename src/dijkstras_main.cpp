#include "dijkstras.h"

int main() {
    try {
        string filename;
        cout << "Enter graph file name: ";
        cin >> filename;
        
        Graph G;
        file_to_graph(filename, G);
        
        const int source = 0;
        vector<int> previous;
        vector<int> distances = dijkstra_shortest_path(G, source, previous);
        
        cout << "Shortest paths from vertex " << source << ":" << endl;
        for (int dest = 0; dest < G.numVertices; ++dest) {
            vector<int> path = extract_shortest_path(distances, previous, dest);
            print_path(path, distances[dest]);
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}