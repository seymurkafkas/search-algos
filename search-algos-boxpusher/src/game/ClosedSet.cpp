#include <ClosedSet.h>

void ClosedSet::markAsVisited(GameState *visitedState)
{
    mapOfVisitedStates[visitedState] = true;
}

bool ClosedSet::isStateVisited(GameState *checkedState)
{
    return mapOfVisitedStates.find(checkedState) != mapOfVisitedStates.end();
}
