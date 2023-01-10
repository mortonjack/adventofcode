#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <ctime>
using std::time;
using std::vector;
using std::unordered_map; using std::queue;
using std::cout; using std::endl;
using std::string; using std::ifstream; using std::to_string;

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

int global_max = 0;
unordered_map<string, int> dp;

string key(int node, int time, bool elephant, vector<bool>& visited) {
    if (!elephant) time += 26;
    string res = to_string(node);
    res += '_';
    res += to_string(time);
    res += '_';
    for (int i = 0; i < (int)visited.size(); i++) res += visited[i] ? '1' : '0';
    return res;
}

// Elephant time
int dfs(int node, int time, bool elephant, vector<vector<int>>& adj, vector<int> weight, vector<bool> visited, int max_score) {
    // Check if this has already been calculated
    string k = key(node, time, elephant, visited);
    if (dp.count(k)) return dp[k];

    // Backtrack if it's impossible to beat the current max - This shaves off 3 seconds. I'll take it!
    int theoretical = weight[node] * (time-1);
    for (int i = 0; i < (int)visited.size(); i++) {
        if (!visited[i]) {
            int best = weight[i] * (time-1-adj[node][i]);
            if (!elephant) {
                int best_el = weight[i] * (25-adj[0][i]);
                best = best > best_el ? best : best_el;
            }
            if (best > 0) theoretical += best;
        }
    }
    if (theoretical < max_score) return 0;

    // Make move
    if (time <= 1) return elephant ? 0 : dfs(0, 26, true, adj, weight, visited, max_score);
    int val = 0;
    if (node != 0) {
        time--;
        val = time * weight[node];
    }
    // Explore the rest of the nodes!
    int max = dfs(node, 0, elephant, adj, weight, visited, 0);
    for (int i = 0; i < (int)visited.size(); i++) {
        if (!visited[i]) {
            visited[i] = true;
            int path = dfs(i, time-adj[node][i], elephant, adj, weight, visited, max);
            max = max > path ? max : path;
            visited[i] = false;
        }
    }
    if (val + max > global_max) {
        global_max = val + max;
        cout << "New max " << global_max << endl;
    }
    return dp[k] = val + max;
}

int main() {
    time_t start_time = time(NULL);
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
        cout << '\n';
    }

    /* STEP 2: Starting at A, time = 30, each travel takes weight long, and opening a valve 
     * takes 1 time, then adds weight*time to the total. Find the path which maximises this 
     * total value. can be brute force at first. Don't visit any node twice. */
    vector<int> w; // these names are confusing!!! oopsies!!!
    vector<bool> visited(count+1, false);
    visited[0] = true;
    for (int i = 0; i < (int)weights.size(); i++) w.push_back(network.weight[weights[i]]);
    int release = dfs(0, 26, false, adj, w, visited, 0);
    time_t end_time = time(NULL);
    cout << "\nThe maximum pressure released is " << release << '\n';
    cout << "Took " << end_time - start_time << " seconds to complete.\n";

    file.close();
    return 0;
}
