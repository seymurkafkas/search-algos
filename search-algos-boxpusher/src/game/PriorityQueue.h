#ifndef GAME_PRIORITYQUEUE_H
#define GAME_PRIORITYQUEUE_H

#include <Frontier.h>
#include <queue>
#include <Hash.h>

class PriorityQueue : public Frontier
{
public:
    bool isEmpty();
    SearchNode *popNextNode();
    void addNode(SearchNode *);

private:
    std::priority_queue<SearchNode *, std::vector<SearchNode *>, NodeComparator> minheap;
};

#endif