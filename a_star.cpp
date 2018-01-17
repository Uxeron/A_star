#include <forward_list>
#include <cmath>
#include "a_star.h"

using namespace std;

#define NULL nullptr

vector< vector<Node*> > grid;
forward_list<Node*>::iterator it1;
forward_list<Node*>::iterator it2;

float costEstimate(pair<int, int> start, pair<int, int> finish);
void  reconstructPath(Node lastNode, vector< pair<int, int> >* path);
void  insertNode(forward_list<Node*> &nodeQueue, Node* node);

void initAStar(int WIDTH, int HEIGHT) {
    grid = vector< vector<Node*> > (WIDTH, vector<Node*>(HEIGHT));
}

float costEstimate(pair<int, int> start, pair<int, int> finish) {
    return sqrt(pow(start.first-finish.first, 2) + pow(start.second-finish.second, 2));
}

void reconstructPath(Node* lastNode, vector< pair<int, int> > &path) {
    while((*lastNode).cameFrom != NULL) {
        path.push_back((*lastNode).position);
        lastNode = (*lastNode).cameFrom;
    }
    path.push_back((*lastNode).position);
    path = vector< pair<int, int> > (path.rbegin(), path.rend());
}

void insertNode(forward_list<Node*> &nodeQueue, Node* node) {
    it2 = nodeQueue.before_begin();
    int nodeFscore = (*node).fScore;
    if(!nodeQueue.empty())
        for(it1 = nodeQueue.begin(); it1 != nodeQueue.end() && (**it1).fScore > nodeFscore; it2 = it1, ++it1) {}
    nodeQueue.insert_after(it2, node);
}

void AStar(vector< pair<int, int> > &path, vector< vector<char> > walls, pair<int, int> start, pair<int, int> finish) {
    pair<int, int> offset[8] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    forward_list<Node*> nodeQueue;
    float tempGScore;
    const float sqr2 = sqrt(2);
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
