#ifndef GAME_FRONTIER_H
#define GAME_FRONTIER_H

#include <SearchNode.h>

class Frontier
{
public:
    virtual bool isEmpty() = 0;
    virtual SearchNode *popNextNode() = 0;
    virtual void addNode(SearchNode *) = 0;

private:
};

#endif