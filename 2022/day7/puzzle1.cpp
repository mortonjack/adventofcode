#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::cout; using std::endl;
using std::string; using std::ifstream;
using std::vector;

#define MAX_LENGTH 50 // max length of lines

/* Tree containing sub-folders & item size */
class Directory {
    public:
        Directory* parent;
        vector<Directory*> subfolders;
        string key; /* name of folder */
        int local_size; /* size of local files */
        int total_size; /* size including subdirectories */

        Directory(string name) {
            parent = nullptr;
            key = name;
            local_size = 0;
            total_size = 0;
        }

        void add_directory(Directory* sub) {
            subfolders.push_back(sub);
            total_size += sub->total_size;
            sub->parent = this;
        }

        void recalculate_size() {
            total_size = local_size;
            for (int i = 0; i < (int)subfolders.size(); i++) {
                subfolders[i]->recalculate_size();
                total_size += subfolders[i]->total_size;
            }
        }

        ~Directory() {
            for (int i = 0; i < (int)subfolders.size(); i++) {
                delete subfolders[i];
            }
        }
};

int dfs(Directory* node) {
    int size = 0;
    for (int i = 0; i < (int)node->subfolders.size(); i++) {
        size += dfs(node->subfolders[i]);
    }
    return node->total_size > 100000 ? size : size+node->total_size;
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int length = line.length();
    Directory* root = new Directory("/");
    Directory* current = root;

    while (!file.eof()) {
        cout << line << endl;
        // Read line
        if (line[0] == '$') { // command
            if (line[2] == 'c') {
                // Change directory
                if (line[5] == '/') {
                    // Set current directory to root
                    current = root;
                } else if (line[5] == '.') {
                    // Set current directory to parent
                    current = current->parent;
                } else {
                    // Check for subfolder with same key or make new folder
                    Directory* old = current;
                    int i = 0;
                    while (current == old) {
                        if (current->subfolders[i]->key == line.substr(5, length-1)) {
                            current = current->subfolders[i];
                        }
                        i++;
                    }
                }
            } else {
                // List directory
            }
        } else { // output of ls
            if (line[0] == 'd') {
                // Subdirectory
                Directory* sub = new Directory(line.substr(4,length-1));
                current->add_directory(sub);
            } else {
                // Local file
                current->local_size += stoi(line);
            }
        }
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
        length = line.length();
    }

    root->recalculate_size();

    // Output root total size
    cout << "The total size of the root directory is " << root->total_size << endl;
    // Use DFS to find sum of sizes
    int sum = dfs(root);
    cout << "The sum of the sizes of directories with less than 100,000 total size is ";
    cout << sum << endl;

    file.close();
    delete root;
    return 0;
}
