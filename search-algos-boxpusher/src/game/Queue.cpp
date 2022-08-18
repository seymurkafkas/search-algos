#include <Queue.h>

bool Queue::isEmpty()
{
    return nodeQueue.empty();
}

void Queue::addNode(SearchNode *nodeToInsert)
{
    nodeQueue.push(nodeToInsert);
}

SearchNode *Queue::popNextNode()
{
    SearchNode *nextNodePtr = nodeQueue.front();
    nodeQueue.pop();
    return nextNodePtr;
}