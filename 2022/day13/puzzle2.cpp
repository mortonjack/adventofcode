#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
using std::vector; using std::sort;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 250 // max length of lines

/* Be warned this is not at all memory safe */

class Pair {
    public:
        bool integer; /* List or Int */
        int val; /* Value (if integer) */
        vector<Pair*> list; /* List of elements (could be integers or lists) */
        bool divider;

        // Constructor
        Pair(int x) {
            integer = true;
            divider = false;
            val = x;
        }
        
        Pair(bool is_integer) {
            divider = false;
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

int base_compare(Pair* left, Pair* right) {
    if (compare(left, right) != -1) return true;
    return false;
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

    vector<Pair*> packets;

    // Add divider packets
    Pair* div1 = new Pair(false);
    string div_1 = "[[2]]";
    string div_2 = "[[6]]";
    int i = 1;
    read(div_1, i, div1);
    packets.push_back(div1);
    Pair* div2 = new Pair(false);
    i = 1;
    read(div_2, i, div2);
    div1->divider = true;
    div2->divider = true;
    packets.push_back(div2);

    while (!file.eof()) {
        index++;

        // Read line
        Pair * current = new Pair(false);
        int i = 1;
        read(line, i, current);

        // Append to vector
        packets.push_back(current);

        // Prepare for next line
        do {
            file.getline(thisLine, MAX_LENGTH);
            line.assign(thisLine);
            count++;
            count %= 3;
        } while (count == 2);
    }

    // Sort packets
    sort(packets.begin(), packets.end(), base_compare);

    // Search for dividers
    int decoder = 1;
    for (int i= 1; i <=(int) packets.size(); i++) {
        if (packets[i-1]->divider) {
            decoder *= i;
        }
    }
    cout << decoder << endl;

    file.close();
    return 0;
}
