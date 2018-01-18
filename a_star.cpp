#include <list>
#include <cmath>
#include "a_star.h"

using namespace std;

#define NULL nullptr

vector< vector<Node*> > grid;
list<Node*>::iterator it1;
const float sqr2 = sqrt(2);

float costEstimate(pair<int, int> start, pair<int, int> finish);
void  reconstructPath(Node lastNode, vector< pair<int, int> >* path);
void  insertNode(list<Node*> &nodeQueue, Node* node);

void initAStar(int WIDTH, int HEIGHT) {
    grid = vector< vector<Node*> > (WIDTH, vector<Node*>(HEIGHT));
}

float costEstimate(pair<int, int> start, pair<int, int> finish) {
    int dist1 = start.first-finish.first;
    int dist2 = start.second-finish.second;
    return min(dist1, dist2)*sqr2 + abs(dist1-dist2);
}

void reconstructPath(Node* lastNode, vector< pair<int, int> > &path) {
    while((*lastNode).cameFrom != NULL) {
        path.push_back((*lastNode).position);
        lastNode = (*lastNode).cameFrom;
    }
    path.push_back((*lastNode).position);
    path = vector< pair<int, int> > (path.rbegin(), path.rend());
}

void insertNode(list<Node*> &nodeQueue, Node* node) {
    int nodeFscore = (*node).fScore;
    for(it1 = nodeQueue.begin(); it1 != nodeQueue.end() && (**it1).fScore > nodeFscore; ++it1) {}
    nodeQueue.insert(it1, node);
}

void AStar(vector< pair<int, int> > &path, vector< vector<char> > walls, pair<int, int> start, pair<int, int> finish) {
    pair<int, int> offset[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    list<Node*> nodeQueue;
    float tempGScore;
    int posx, posy;
    pair<int, int> position;
    int WIDTH  = grid.size();
    int HEIGHT = grid[0].size();
    vector< vector<char> > inQueue(WIDTH, vector<char>(HEIGHT, false));

    Node startNode;
    Node* currentNode;
    Node* neighbourNode;

    startNode.position = start;
    startNode.cameFrom = NULL;
    startNode.gScore = 0.0;
    startNode.fScore = costEstimate(start, finish);
    grid[start.first][start.second] = &startNode;

    nodeQueue.push_front(&startNode);

    while(!nodeQueue.empty()) {
        currentNode = nodeQueue.front();
        position = (*currentNode).position;
        nodeQueue.pop_front();
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
                insertNode(nodeQueue, neighbourNode);
            }

            tempGScore = (*currentNode).gScore + (i%2==0 ? sqr2 : 1);
            if(tempGScore >= (*neighbourNode).gScore) continue;

            (*neighbourNode).cameFrom = currentNode;
            (*neighbourNode).gScore = tempGScore;
            (*neighbourNode).fScore = tempGScore + costEstimate((*neighbourNode).position, finish);
        }
    }
}
