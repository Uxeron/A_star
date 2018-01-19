#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "a_star.h"

#define debug 0
#define test  1   //0 - long path; 1 - large amount of small paths

#if test
    #define inpFile "mapFile1.txt"
#else
    #define inpFile "mapFile3.txt"
#endif

#define outFile "mapFilePath.txt"

using namespace std;

void outputPath(vector< pair<int, int> > path, int WIDTH, int HEIGHT);

int main() {
    int HEIGHT, WIDTH;
    pair<int, int> start;
    pair<int, int> finish;
    vector< pair<int, int> > path;

    ifstream fd(inpFile);

    if(debug) cout << "reading parameters" << endl;

    fd >> WIDTH >> HEIGHT >> start.first >> start.second >> finish.first >> finish.second;
    char inputStr[WIDTH+1];

    if(debug) cout << "allocating memory for node grid" << endl;

    vector< vector<char> > walls (WIDTH, vector<char>(HEIGHT, false));

    if(debug) cout << "parsing map file" << endl;

    // Parse map file
    for(int i = 0; i < HEIGHT; i++) {
        fd >> inputStr;
        for(int j = 0; j < WIDTH; j++) {
            if(inputStr[j] != '.') {
                walls[j][i] = true;
            }
        }
        fd.ignore();
    }
    fd.close();

    if(debug) cout << "Finding path" << endl;
    initAStar(WIDTH, HEIGHT);
    if(!test) {
        AStar(path, walls, start, finish);
    } else {
        for(int i = 0; i < 5000; i++) {
            AStar(path, walls, start, finish);
            path.clear();
        }
    }
    if(debug) cout << endl;
    if(debug) cout << "Path length - " << path.size() << endl;

    if(debug) outputPath(path, WIDTH, HEIGHT);

    return 0;
}

void outputPath(vector< pair<int, int> > path, int WIDTH, int HEIGHT) {
    struct PairCompare {
        bool operator() (pair<int, int> n1, pair<int, int> n2) {
            if(n1.second == n2.second)
                return n1.first < n2.first;
            else
                return n1.second < n2.second;
        }
    };

    cout << "exporting path" << endl;
    int thrash;
    unsigned int index = 0;
    char inpChar[WIDTH+1];
    ifstream fd(inpFile);
    ofstream fr(outFile);

    for(int i = 0; i < 6; i++) fd >> thrash;

    cout << "sorting path" << endl;
    sort(path.begin(), path.end(), PairCompare());
    cout << "writing to file" << endl;

    for(int y = 0; y < HEIGHT; y++) {
        fd >> inpChar;
        while(index < path.size() && path[index].second == y) {
            inpChar[path[index].first] = '*';
            index++;
        }
        fr << inpChar << "\r\n";
    }
    fd.close();
    fr.close();
}
