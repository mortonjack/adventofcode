#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using std::vector;
using std::cout; using std::endl;
using std::string; using std::ifstream;

#define MAX_LENGTH 10 // max length of lines

class llnode {
    public:
        llnode* next;
        llnode* prev;
        long long val;
        int count;

        int index; // initial index
        llnode(long long value, int i) {
            val = value;
            index = i;
        }

        void swap() {
            int loop = val%(count-1);
            if (loop == 0) return;
            
            llnode* pos = this;
            llnode* after = this;
            
            // Set nodes before/after to point to each other
            prev->next = next;
            next->prev = prev;
           
            // Get node at new position
            if (val > 0) {
                for (int i = 0; i < loop; i++) pos = pos->next;
                after = pos->next;
            } else {
                for (int i = loop; i < 0; i++) pos = pos->prev;
                after = pos;
                pos = pos->prev;
            }
            
            // Set current node between pos and the node after pos
            pos->next = this;
            after->prev = this;
            next = after;
            prev = pos;
        }

};

int main() {
    ifstream file;
    file.open("input.txt");

    char thisLine[MAX_LENGTH];
    string line;
    file.getline(thisLine, MAX_LENGTH);
    line.assign(thisLine);
    vector<llnode*> nodes;
    int count = 0;
    int index = -1;
    
    long long key = 811589153;

    while (!file.eof()) {
        nodes.push_back(new llnode(stoi(line)*key, count));
        count++;
        
        // Prepare for next line
        file.getline(thisLine, MAX_LENGTH);
        line.assign(thisLine);
    }
	if (count != (int)nodes.size()) {
		cout << "ERROR: Count != nodes.size()" << endl;
		return -1;
	}

    // Print thingo
    for  (int i = 0; i < count-1; i++) {
        if (nodes.at(i)->val == 0) index = i;
        nodes.at(i)->next = nodes.at(i+1);
        nodes.at(i+1)->prev = nodes.at(i);
        nodes.at(i)->count = count;
        //cout << nodes.at(i)->val << " -> ";
    }
    nodes.at(0)->prev = nodes.at(count-1);
    nodes.at(count-1)->next = nodes.at(0);
    if (nodes.at(count-1)->val == 0) index = count-1;
    nodes.at(count-1)->count = count;
    //cout << nodes.at(count-1)->val <<  endl;
    if (index == -1) {
        cout << "ERROR: NO 0 INDEX FOUND\n";
        return -1;
    }

    /*llnode* p = nodes.at(6);
    for (int i = 0; i < count-1; i++) {
        cout << p->val << " <- ";
        p = p->prev;
    }
    cout << p->val << endl;*/


    // Set new positions
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < count; i++) {
            nodes.at(i)->swap();
        }
    }
    
    /*llnode* curr = nodes.at(0);
    for (int i = 0; i < count-1; i++) {
        cout << curr->val << " -> ";
        curr = curr->next;
    }
    cout << curr->val << endl;*/
    
    long long a = 0, b = 0, c = 0;
    llnode* curr = nodes.at(index);
    for (int i = 0; i < 3001; i++) {
        if (i == 1000) a = curr->val;
        if (i == 2000) b = curr->val;
    	if (i == 3000) c = curr->val;
        curr = curr->next;
    }

    cout << "0th is " << nodes.at(index)->val << endl;
    cout << "1000th is " << a << endl;
    cout << "2000th is " << b << endl;
    cout << "3000th is " << c << endl;
    cout << "Adding together makes " << a+b+c << endl;

    file.close();
    return 0;
}
