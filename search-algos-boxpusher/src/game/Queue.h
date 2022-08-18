#ifndef GAME_QUEUE_H
#define GAME_QUEUE_H

#include <Frontier.h>
#include <SearchNode.h>
#include <queue>

class Queue : public Frontier
{
public:
    bool isEmpty();
    SearchNode *popNextNode();
    void addNode(SearchNode *);

private:
    std::queue<SearchNode *> nodeQueue;
};

#endif