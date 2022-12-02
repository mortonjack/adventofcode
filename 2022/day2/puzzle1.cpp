#include <iostream>
#include <fstream>
#include <string>
using std::cout; using std::endl;
using std::ifstream; using std::string;

#define pROCK 'X'
#define pPAPER 'Y'
#define pSCISSORS 'Z'
#define eROCK 'A'
#define ePAPER 'B'
#define eSCISSORS 'C'

int main() {
    ifstream file;
    file.open("input.txt");
    
    char thisLine[5];
    file.getline(thisLine, 4);
    int score = 0;
    int shape = 0;
    int outcome = 0;

    char player;
    char enemy;

    while (!file.eof()) {
        player = thisLine[2];
        enemy = thisLine[0];

        if (player == pROCK) {
            shape = 1;
            if (enemy == eROCK) outcome = 3;
            else if (enemy == ePAPER) outcome = 0;
            else if (enemy == eSCISSORS) outcome = 6;
        } else if (player == pPAPER) {
            shape = 2;
            if (enemy == eROCK) outcome = 6;
            else if (enemy == ePAPER) outcome = 3;
            else if (enemy == eSCISSORS) outcome = 0;
        } else if (player == pSCISSORS) {
            shape = 3;
            if (enemy == eROCK) outcome = 0;
            else if (enemy == ePAPER) outcome = 6;
            else if (enemy == eSCISSORS) outcome = 3;
        }
        
        score += outcome + shape;
        file.getline(thisLine, 4);
    }

    cout << "Total score: " << score << endl;

    file.close();
}
