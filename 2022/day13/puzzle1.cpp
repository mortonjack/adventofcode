#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 250 // max length of lines

class Pair {
    public:
        bool integer; /* List or Int */
        int val; /* Value (if integer) */
        vector<Pair*> list; /* List of elements (could be integers or lists) */

        // Constructor
        Pair(int x) {
            integer = true;
            val = x;
        }
        
        // Destructor
        ~Pair() {
            for (int i = 0; i < (int)list.size(); i++) {
                delete list[i];
            }
        }

        Pair(bool is_integer) {
            integer = is_integer;
            val = -1;
        }

        void insert(int x) {
            if (integer) val = x;
            else {
                if (x == -1) list.push_back(new Pair(false)); /* Add new list to list */
                else list.push_back(new Pair(x)); /* Add new int to list */
            }
        }
};

int compare(Pair* left, Pair* right) {
    /* Returns true if sorted correctly */

    if (left->integer && right->integer) {
        /* Both integers */
        if (left->val > right->val) {
            return -1;
        }
        else if (left->val < right->val) {
            return 1;
        }
    } else if (!left->integer && !right->integer) {
        /* Neither integers - compare each value of their lists */
        for (int i = 0; i < (int)left->list.size(); i++) {
            /* Return false if right runs out of items first */
            if (!(i < (int)right->list.size())) {
                return -1;
            }
            /* Return false if unsorted (recursion) */
            int c = compare(left->list[i], right->list[i]);
            if (c != 0) return c;
        }
        /* Return 1 if right still has items */
        if (left->list.size() != right->list.size()) {
            return 1;
        }
    } else if (left->integer) {
        /* Only left integer */
        if (right->list.size() == 0) {
            return -1;
        }
        return compare(left, right->list[0]);
    } else {
        /* Only right integer */
        if (left->list.size() == 0) {
            return 1;
        }
        /* Only 1 element in list */
        int comparison = compare(left->list[0], right);
        if (left->list.size() == 1) {
            return comparison;
        }
        /* Multiple element in list */
        return comparison != 0 ? comparison : -1;
    }

    // No errors found, return 0
    return 0;
}

void read(string& line, int& i, Pair * current_pair) {
    // Read contents of line into pair
    while (i < (int)line.length()) {
        if (line[i] == '[') {
            // New list
            Pair* new_list = new Pair(false);
            current_pair->list.push_back(new_list);
            // Read contents of list into pair
            read(line, ++i, new_list);
        } else if (line[i] == ']') {
            i++;
            // End list
            return;
        } else if (line[i] == ',') {
            i++;
        } else {
            // Append integer to back of list
            int current = stoi(line.substr(i, i+2));
            if (current > 9) i++;
            current_pair->list.push_back(new Pair(current));
            i++;
        }
    }
}

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    int count = 0; // 0 = left, 1 = right; 2 = gappppp
    int index = 0;

    Pair * left;
    Pair * right;

    int sum = 0;

    while (!file.eof()) {
        if (count == 0) index++;

        // Read line
        Pair * current = new Pair(false);
        int i = 1;
        read(line, i, current);
        if (count == 0) left = current;
        else right = current;

        // Compare pair
        if (current == right) {
            int comparative = compare(left, right);
            if (comparative != -1) {
                sum += index;
            }
        }

        // Prepare for next line
        do {
            file.getline(thisLine, MAX_LENGTH);
            line.assign(thisLine);
            count++;
            count %= 3;
        } while (count == 2);
    }

    cout << sum << endl;

    file.close();
    return 0;
}
