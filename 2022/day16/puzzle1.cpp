#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
using std::vector;
using std::unordered_map; using std::queue;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 100 // max length of lines

struct EdgeNode {
    string key;
    EdgeNode* next;
    int weight;
    EdgeNode(string key) : key(key), next(nullptr), weight(0) {}
    EdgeNode(string key, int n) : key(key), next(nullptr), weight(n) {}
};

class Graph {
    public: 
        unordered_map<string, EdgeNode*> adj; /* adjacency list */
        unordered_map<string, int> weight; /* weight */
        unordered_map<string, bool> opened; /* valve opened */

        Graph() {}

        void add(string key, int w) {
            weight[key] = w;
            opened[key] = false;
        }

        void insert_edge(string key1, string key2) {
            EdgeNode* next = adj[key1];
            if (next == nullptr) {
                adj[key1] = new EdgeNode(key2);
                return;
            }

            while (next->next != nullptr) {
                next = next->next;
            }
            next->next = new EdgeNode(key2);
        }
};

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();
    Graph network;
    
    string root = "AA";
    vector<string> weights;
    weights.push_back(root);
    int count = 0;

    /* Read input from file */
    while (!file.eof()) {
        string key = line.substr(6, 2);
        int weight = stoi (line.substr(23, 4));
        if (weight > 0) {
            count++;
            weights.push_back(key);
        }

        network.add(key, weight);

        int i = 47;
        while (line[i] != ' ') i++;

        for (int j = i+1; j < length; j += 4) {
            string adj = line.substr(j, 2);
            network.insert_edge(key, adj);
        }

        /* Print connections
        EdgeNode* next = network.adj[key];
        cout << key << ": ";
        while (next != nullptr) {
            cout << next->key << " ";
            next = next->next;
        }
        cout << endl; */
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    /* GOAL:
     * Find shortest path from A that visits each weighted node weight > 0. 
     * Also find shortest path from each weighted node weight > 0 to each other.
     * Then I can set up a smaller graph with only weighted nodes, starting at A, that
     * I can brute-force my way through. Hopefully, it will be small enough 
     * that this won't be so bad. */

    vector<vector<int>> adj(count+1, vector<int> (count+1, 0)); // adjacency graph
    unordered_map<string, int> index; /* get index */
    for (int i = 0; i < (int)weights.size(); i++) {
        index[weights[i]] = i;
    }

    /* STEP 1: Use Breadth-First-Search to discover each weighted node. */
    for (int i = 0; i < (int)weights.size(); i++) {
        root = weights[i];
        string node = root;
        unordered_map<string, int> discovered;
        discovered[node] = 0;
        queue<string> next;
        next.push(node);

        int found = 0;
        int depth = 100;

        while (found < count) {
            node = next.front();
            next.pop();
            depth = discovered[node];

            // Process node
            if (network.weight[node] != 0) {
                adj[index[node]][index[root]] = depth;
                adj[index[root]][index[node]] = depth;
                found++;
            }

            // Add children to queue
            EdgeNode* edge = network.adj[node];
            while (edge != nullptr) {
                if (discovered.count(edge->key) == 0) {
                    discovered[edge->key] = depth+1;
                    next.push(edge->key);
                }
                edge = edge->next;
            }
        }
    }
    
    /* Output adjacency matrix */
    cout << "    ";
    for (int i = 0; i < (int)adj.size(); i++) cout <<  weights[i] << " ";
    cout << endl;
    for (int i = 0; i < (int)adj.size(); i++) {
        cout << weights[i] << ": ";
        for (int j = 0; j < (int)adj[i].size(); j++) {
            cout << adj[i][j] << " ";
            if (adj[i][j] < 10) cout << " ";
        }
        cout << endl;
    }

    file.close();
    return 0;
}
