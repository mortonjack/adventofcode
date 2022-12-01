#include <iostream>
#include <string>
#include <fstream>
using std::cout; using std::endl;
using std::string;
using std::ifstream;
using std::stoi;

class MaxHeap {
    private:
        int * array;
        int capacity;
        int items;

        void doubleSize() {
            capacity *= 2;
            int *p = new int[capacity+1]; /* temp pointer */
            for (int i = 1; i <= items; i++) {
                p[i] = array[i]; /* copy items to new array */
            }
            delete [] array;
            array = p;
        }
    public:
        MaxHeap() {
            items = 0;
            capacity = 8; /* initialising array to hold 8 items */
            array = new int[capacity+1];
        }

        void print() { /* for testing! */
            for (int i = 1; i <= items; i++) {
                cout << array[i] << " ";
            }
            cout << endl;
        }

        int pop() {
            /* Delete & return max item from top of the tree */
            int max = array[1];
            array[1] = array[items];
            items--;
            bubbleDown(1);
            return max;
        }

        int look() {
            /* Return max item from top of the tree */
            return array[1];
        }

        void insertWithoutSorting(int item) {
            if (items == capacity) doubleSize();
            items++;
            array[items] = item;
        }

        void insert(int item) {
            insertWithoutSorting(item);
            bubbleUp(items);
        }

        void bubbleDown(int index) {
            /* Bubble down item at given index until it is larger than both
             * its children */

            // Check item isn't a leaf node
            if (index*2 > items) return;

            int parent = array[index];
            int left = array[index*2];
            int right = index*2+1 <= items ? array[index*2 + 1] : -1;
            int iChild;

            // Check if item is smaller than its children
            if (parent < left || parent < right) {
                // Swap parent with largest child
                iChild = left > right ? index*2 : index*2 + 1;
                array[index] = array[iChild];
                array[iChild] = parent;
                // Recurse downwards
                bubbleDown(iChild);
            }
        }

        void bubbleUp(int index) {
            /* Bubble up item at given index until it is smaller
             * than its parent */
            
            if (index == 1) return; // end recursion
            
            int item = array[index];
            int iParent = index / 2;

            // Check if item is larger than its parent
            if (item > array[iParent]) {
                // Swap with parent
                array[index] = array[iParent];
                array[iParent] = item;
                // Recurse upwards
                bubbleUp(iParent);
            }
        }

        void resort() {
            for (int i = items; i > 0; i--) {
                bubbleDown(i);
            }
        }
};

int main() {
    // open file
    ifstream file;
    file.open("input1.txt");

    char line[10]; // store current line
    file.getline(line, 9);
    int food; // stores food carried by this elf
    string thisLine;
    MaxHeap max; // stores all elf food amounts

    while (!file.eof()) {
        if (line[0] == '\0') {
            max.insertWithoutSorting(food);
            food = 0;
        } else {
            if (food > 100000) food = 0; // sometimes this glitches idk
            thisLine.assign(line);
            food += stoi(thisLine, nullptr, 10);
        }
        file.getline(line, 9);
    }

    max.resort();
    
    cout << "The total calories carried by the top 3 elves is ";
    int a = max.pop();
    int b = max.pop();
    int c = max.pop();
    cout << a << " " << b << " " << c << endl;
    cout << a + b + c << " calories." << endl;

    // close file
    file.close();
    return 0;
}
