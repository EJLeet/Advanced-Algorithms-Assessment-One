#include <iostream>
#include <set>
#include <map>
#include <fstream>

using std::cout;
using std::endl;

class EfficientAdjacencyList
{
    // structure to hold the adjacency list
    std::map<int, std::set<int>> adj_list;

    // variables relating to individual graphs
    int edges = 0, vertices = 0;
    double density = 0.;
    bool sparse = false;

    // construct a sparse graph
    void construct_sparse(std::ifstream &file);
    
    // construct a dense graph
    void construct_dense(std::ifstream &file, double vertices);

public:

    // determine graph and appropriately load based on density
    void determine(char* filename);

    // returns if vertex v is ocnnected to u
    bool connected(int v, int u);

    // returns the list of neighbours to v
    std::set<int> get_neighbours(int v);
};

void EfficientAdjacencyList::determine(char* filename)
{/* 
        This function takes the file passed from the 
        command line and determines what type of graph
        is needed. It determines density which we then
        use to construct an adjancy list for either a
        sparse or dense graph.
                                                            */

    std::ifstream file(filename);

    // read number of vertices and edges
    file >> vertices >> edges;

    // calculate density
    density = edges / (vertices * (vertices - 1));

    /*
        we have a sparse graph is density is less than 0.5
        we have a dense graph otherwise                     
                                                            */ 
    if (density < 0.5)
        construct_sparse(file);

    else
        construct_dense(file, vertices);
}

void EfficientAdjacencyList::construct_sparse(std::ifstream &file)
{/*
        This function reads the file into a map. In the map
        the key is the vertex and the value is the set of
        edges that vertex points to.
                                                                */
    sparse = true;
    int v, u;
    while (file >> v >> u)
        adj_list[v].emplace(u);
}

void EfficientAdjacencyList::construct_dense(std::ifstream &file, double vertices)
{/*
        This function reads the file vertices times. Each
        iteration it holds all the edges for that current
        index. It then adds the inverse connections to 
        the map.
                                                                */
    sparse = false;
    int v, u;
    for (int i = 0; i < vertices; i++)
    {// loop through each vertex

        // initialise temp set to hold edges
        std::set<int> temp;

        //  loop until end of file
        while (!file.eof())
        {// read each vertex/edge
            file >> v >> u;
            // if v == i add u to temp set
            if (v == i)
                temp.emplace(u);
        }

        /*
            Loop through searching from 0 to vertices in 
            temp. If the index is not the vertex we are at 
            and the index is not in temp add it to the 
            adj_list map.
                                                                */
        for (int j = 0; j < vertices; j++)
            if ((j != i) && (!temp.count(j)))
                adj_list[i].emplace(j);
        
        // go back to beginning of file (after vertices and edges)
        file.seekg(4, std::ios::beg);
    }
}

bool EfficientAdjacencyList::connected(int v, int u)
{/*
        This function performs a binary search of u on some 
        vertex v for some sparse or dense graph. If the graph
        is sparse it returns true if u is found. If the graph
        is dense it returns false.
                                                                */
    if (sparse) // graph is sparse
        return std::binary_search(adj_list.at(v).begin(), 
                                  adj_list.at(v).end(), u);

    else // graph is dense
        return std::binary_search(adj_list.at(v).begin(), 
                                  adj_list.at(v).end(), u) ? 0 : 1;
}

std::set<int> EfficientAdjacencyList::get_neighbours(int v)
{/*
        This function returns the neighbours (edges) of some
        vertex v. If the graph is sparse it simply prints
        the values of the map at v. If it is dense it loops
        from 0 to the number of vertices and prints any value
        not present in the values of the map at v.
                                                                */
    std::set<int> neighbours;

    if (sparse)
    {// graph is sparse

        auto it = adj_list.at(v);
        for (auto i : it)
            neighbours.emplace(i);
    }

    else
    {// graph is dense

        auto it = adj_list.at(v);
        for (int i = 0; i <= adj_list.size(); i++)       
            if (!it.count(i) && i != v)
                neighbours.emplace(i);
    }
    return neighbours;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {// ensure filename is passed

        cout << "ERROR! Expected filename" << endl;
        exit(1);
    }

    // create object
    EfficientAdjacencyList graph;

    // load graph into object
    graph.determine(argv[1]);

    // Set u and v for below tests
    int v = 4, u = 6;

    // Is vertex v vonnected to vertex u
    cout << "Testing: Is vertex " << v << " connected to vertex " << u << "?" << endl;
    cout << (graph.connected(v, u) == 1 ? "True" : "False") << endl;
    
    // Produce a list of all vertices connected to v
    cout << "Testing: Produce a list of all vertices connected to " << v << "..." << endl;
    std::set<int> neighbours = graph.get_neighbours(v);

    for (auto it : neighbours)
        cout << it << " ";
    cout << endl;

    return 0;
}