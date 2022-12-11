#include <iostream>
#include <queue>
#include <vector>
using std::queue; using std::vector;
using std::cout; using std::endl;

/* Small, complicated input... faster to just enter everything in myself! */

class Monkey {
    public:
        queue<int> worry_level;
        int id;

        // Constructor
        Monkey(int i) {
            id = i;
            vector<int> items;

            if (i == 0) {
                items = {83, 62, 93};
            } else if (i == 1) {
                items = {90, 55};
            } else if (i == 2) {
                items = {91, 78, 80, 97, 79, 88};
            } else if (i == 3) {
                items = {64, 80, 83, 89, 59};
            } else if (i == 4) {
                items = {98, 92, 99, 51};
            } else if (i == 5) {
                items = {68, 57, 95, 85, 98, 75, 98, 75};
            } else if (i == 6) {
                items = {74};
            } else {
                items = {68, 64, 60, 68, 87, 80, 82};
            }

            for (int i = 0; i < items.size(); i++) {
                worry_level.push(items.at(i));
            }
        }

        int operation(int old) {
            switch (id) {
                case 0:
                    return old*17;
                    break;
                case 1:
                    return old + 1;
                    break;
                case 2:
                    return old + 3;
                    break;
                case 3:
                    return old + 5;
                    break;
                case 4:
                    return old*old;
                    break;
                case 5:
                    return old + 2;
                    break;
                case 6:
                    return old + 4;
                    break;
                case 7:
                    return old * 19;
                    break;
            }
            return -1;
        }

        bool test(int n) {
            switch (id) {
                case 0:
                    return n % 2 == 0;
                    break;
                case 1:
                    return n % 17 == 0;
                    break;
                case 2:
                    return n % 19 == 0;
                    break;
                case 3:
                    return n % 3 == 0;
                    break;
                case 4:
                    return n % 5 == 0;
                    break;
                case 5:
                    return n % 13 == 0;
                    break;
                case 6:
                    return n % 7 == 0;
                    break;
                case 7:
                    return n % 11 == 0;
                    break;
            }
            return false;
        }

        int throw_to(bool res) {
            switch (id) {
                case 0:
                    return res ? 1 : 6;
                case 1:
                    return res ? 6 : 3;
                case 2:
                    return res ? 7 : 5;
                case 3:
                    return res ? 7 : 2;
                case 4:
                    return res ? 0 : 1;
                case 5:
                    return res ? 4 : 0;
                case 6:
                    return res ? 3 : 2;
                case 7:
                    return res ? 4 : 5;
            }
            return -1;
        }
};

int main() {
    Monkey* monkey[8]; // array of 8 monkeys
    for (int i = 0; i < 8; i++) {
        monkey[i] = new Monkey(i);
    }

    int inspect[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 8; j++) {
            while (!monkey[j]->worry_level.empty()) {
                // Dequeue item
                int level = monkey[j]->worry_level.front();
                monkey[j]->worry_level.pop();
                
                // Carry out operation
                level = monkey[j]->operation(level);
                
                // Divide by 3
                level /= 3;
                
                // Test
                bool res = monkey[j]->test(level);
                
                // Throw
                monkey[monkey[j]->throw_to(res)]->worry_level.push(level);

                // Increase inspection count
                inspect[j]++;
            }
        }
    }

    int max = 0;
    int multiple = 1;
    for (int i = 0; i < 8; i++) {
        cout << "Monkey " << i << " inspected " << inspect[i] << " times\n";
        if (i != 0 && inspect[i] > inspect[max]) {
            multiple = inspect[max]*inspect[i];
            max = i;
        }
        delete monkey[i];
    }
    cout << multiple << endl;

    return 0;
}
