#include <PriorityQueue.h>

bool PriorityQueue::isEmpty()
{
    return minheap.empty();
}

void PriorityQueue::addNode(SearchNode *nodeToInsert)
{
    minheap.push(nodeToInsert);
}

SearchNode *PriorityQueue::popNextNode()
{
    SearchNode *nextNodePtr = minheap.top();
    minheap.pop();
    return nextNodePtr;
}