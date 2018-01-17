#ifndef A_STAR_H_
    #define A_STAR_H_

    #define FLOAT_MAX 2147483647.0

    #include <utility>
    #include <vector>

    struct Node {
        bool checked = false;
        bool inQueue = false;
        Node *cameFrom;
        std::pair<int, int> position;
        float gScore = FLOAT_MAX;
        float fScore = FLOAT_MAX;
    };

    void AStar(std::vector< std::pair<int, int> >* path, std::vector< std::vector<Node> > grid, std::pair<int, int> start, std::pair<int, int> finish);
#endif
