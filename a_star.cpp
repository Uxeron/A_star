#include <queue>
#include <cmath>
#include "a_star.h"

using namespace std;

#define NULL nullptr

struct NodeCompare {
    bool operator() (Node*& n1, Node*& n2) {
        return (*n1).fScore > (*n2).fScore;
    }
};

vector< vector<Node*> > grid;

float costEstimate(pair<int, int> start, pair<int, int> finish);
void  reconstructPath(Node lastNode, vector< pair<int, int> >* path);


void initAStar(int WIDTH, int HEIGHT) {
    grid = vector< vector<Node*> > (WIDTH, vector<Node*>(HEIGHT));
}

float costEstimate(pair<int, int> start, pair<int, int> finish) {
    return sqrt(pow(start.first-finish.first, 2) + pow(start.second-finish.second, 2));
}

void reconstructPath(Node* lastNode, vector< pair<int, int> >* path) {
    while((*lastNode).cameFrom != NULL) {
        (*path).push_back((*lastNode).position);
        lastNode = (*lastNode).cameFrom;
    }
    (*path).push_back((*lastNode).position);
    (*path) = vector< pair<int, int> > ((*path).rbegin(), (*path).rend());
}

void AStar(vector< pair<int, int> >* path, vector< vector<bool> > walls, pair<int, int> start, pair<int, int> finish) {
    pair<int, int> offset[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    priority_queue<Node*, vector<Node*>, NodeCompare> nodeQueue;
    float tempGScore;
    const float sqr2 = sqrt(2);
    int posx, posy;
    pair<int, int> position;
    int WIDTH  = grid.size();
    int HEIGHT = grid[0].size();
    vector< vector<bool> > inQueue(WIDTH, vector<bool>(HEIGHT, false));

    Node startNode;
    Node* currentNode;
    Node* neighbourNode;

    startNode.position = start;
    startNode.cameFrom = NULL;
    startNode.gScore = 0.0;
    startNode.fScore = costEstimate(start, finish);
    grid[start.first][start.second] = &startNode;

    nodeQueue.push(&startNode);

    while(!nodeQueue.empty()) {
        currentNode = nodeQueue.top();
        position = (*currentNode).position;
        nodeQueue.pop();
        walls[position.first][position.second] = true;

        if(finish == position) {
            reconstructPath(currentNode, path);
            return;
        }

        for(int i = 0; i < 8; i++) {
            posx = position.first  + offset[i].first;
            posy = position.second + offset[i].second;
            if(posx < 0 || posx >= WIDTH || posy < 0 || posy >= HEIGHT) continue;

            if(walls[posx][posy]) continue;
            if(grid[posx][posy] == NULL) grid[posx][posy] = new Node;
            neighbourNode = grid[posx][posy];

            if(!inQueue[posx][posy]) {
                (*neighbourNode).position = {posx, posy};
                inQueue[posx][posy] = true;
                nodeQueue.push(neighbourNode);
            }

            tempGScore = (*currentNode).gScore + (i%2==0 ? sqr2 : 1);
            if(tempGScore >= (*neighbourNode).gScore) continue;

            (*neighbourNode).cameFrom = currentNode;
            (*neighbourNode).gScore = tempGScore;
            (*neighbourNode).fScore = tempGScore + costEstimate((*neighbourNode).position, finish);
        }
    }
}
