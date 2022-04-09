#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using std::endl;
using std::cout;

struct Edge 
{ 
    int source, dest, weight; 
    Edge(int source, int dest, int weight)
            : source(source), dest(dest), weight(weight) {}
};

std::tuple<std::vector<Edge>, int, int> readfile(char* filename, int v, int e);
void bellman_ford(std::vector<Edge> graph, int vertices, int edges);

int main(int argc, char** argv)
{
    if (argc < 2)
    {// ensure filename is passed

        cout << "ERROR! Expected filename" << endl;
        exit(1);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // load graph
    std::vector<Edge> graph;
    int vertices, edges;
    std::tie(graph, vertices, edges) = readfile(argv[1], 0, 0);

    // perform Bellman-Ford
    bellman_ford(graph, vertices, edges);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();
    
    cout << "Program took " << duration << " milliseconds" << endl;

    return 0;
}

std::tuple<std::vector<Edge>, int, int> readfile(char* filename, int v, int e)
{// loads graph into data structure
    std::vector<Edge> graph;
    std::ifstream file(filename);
    std::string line;

    file >> v >> e;
    while (std::getline(file, line))
    {// read points inputed as source, dest, weight

        std::stringstream linestream(line); 
        int s, d, w;
        if(!(linestream >> s >> d >> w)) continue; // handle whitespace after vertex/edge
        graph.push_back(Edge(s, d, w));
    }

    return {graph, v, e};
}

void bellman_ford(std::vector<Edge> graph, int vertices, int edges)
{
    // initialise all distances as infinity
    std::vector<int> distance(vertices, INT_MAX); 
    distance[0] = 0; // source to source is 0 weight

    // Relax edges |V| - 1 times to get shortest path from src
    for (int i = 1; i <= vertices - 1; i++) 

        for (int j = 0; j < edges; j++) 

            if (distance[graph[j].source] != INT_MAX 
                && distance[graph[j].source] + graph[j].weight 
                < distance[graph[j].dest])

                distance[graph[j].dest] = distance[graph[j].source] 
                + graph[j].weight;

    // check for negative weight cycles
    for (int i = 0; i < edges; i++)

        if (distance[graph[i].source] != INT_MAX
            && distance[graph[i].source] + graph[i].weight 
            < distance[graph[i].dest])

            cout << "Graph contains a negative weighr cycle" << endl;
    
    // print distances
    for (int i = 0; i < vertices; i++)

        cout << "Shortest Path from Source to Vertex " << i 
             << " = " << distance[i] << endl;
}

