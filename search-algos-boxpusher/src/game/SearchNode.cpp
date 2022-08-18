
#include <SearchNode.h>
#include <Frontier.h>
#include <ClosedSet.h>
#include <utility>
#include <vector>

SearchNode::SearchNode()
{
    Benchmark::increaseNodeCountInMemory();
}

SearchNode::SearchNode(ClosedSet *visitedSet, Frontier *frontier, SearchNode *parentNode, std::vector<std::pair<int, int>> *boxGoalPositionsPrev, GameState *state, char action, int transitionCost, int heuristicCost = 0)
{
    Benchmark::increaseNodeCountInMemory();
    this->visitedSet = visitedSet;
    parent = parentNode;
    pathCost = (parent ? parent->pathCost : 0) + transitionCost;
    totalCost = pathCost + heuristicCost;
    this->heuristicCost = heuristicCost;
    lastTransitionAction = action;
    currentState = state;
    boxGoalPositions = parent ? parent->boxGoalPositions : boxGoalPositionsPrev;
    this->frontier = frontier;
}

SearchNode::~SearchNode()
{
    Benchmark::decreaseNodeCountInMemory();
    delete currentState;
}
void SearchNode::writeSolutionToFile(std::string fileName)
{
    FileHandler::writeStepsToFile(this, fileName);
}

bool SearchNode::isGoalReached()
{
    return currentState->isGoalReached(*boxGoalPositions);
}

/**
 *      Defines the priority of the nodes, and includes the tie-breaker
 * @param rhs reference to the compared value
 * @return True if the compared value is prioritised in the frontier.
 */
bool SearchNode::operator<(const SearchNode &rhs)
{
    if (this->totalCost == rhs.totalCost)
        return this->heuristicCost > rhs.heuristicCost;
    else
        return this->totalCost > rhs.totalCost;
}

void SearchNode::expandNode()
{
    addEntitiesToRawMap();
    Benchmark::increaseNumberOfNodeExpansions();
    for (char &consideredAction : Actions::getActionSet())
    {
        if (isActionValid(consideredAction))
        {
            SearchNode *successor = generateSuccessorNode(consideredAction, true);

            if (successor)
                frontier->addNode(successor);
        }
    }
    removeEntitiesFromUpdatedMap();
}

void SearchNode::expandNodeAStar()
{
    addEntitiesToRawMap();
    Benchmark::increaseNumberOfNodeExpansions();
    for (char &consideredAction : Actions::getActionSet())
    {
        if (isActionValid(consideredAction))
        {
            SearchNode *successor = generateAStarSuccessorNodeIfUnvisited(consideredAction);
            if (successor)
                frontier->addNode(successor);
        }
    }
    removeEntitiesFromUpdatedMap();
}

/**
 *
 * Dynamically allocates the next search node, using the transition function
 *
 * @param action Container whose values are summed.
 * @return Pointer to the next node that results from action
 */

SearchNode *SearchNode::generateSuccessorNode(char action, bool checkIfVisited)
{
    GameState *nextState = currentState->generateNextState(action);
    if (checkIfVisited)
    {
        bool shouldSkipGeneration = this->visitedSet->isStateVisited(nextState);
        if (shouldSkipGeneration)
        {
            delete nextState;
            return nullptr;
        }
        else
        {
            visitedSet->markAsVisited(nextState);
        }
    }

    int transitionCost = 1;

    return new SearchNode(visitedSet, frontier, this, boxGoalPositions, nextState, action, transitionCost, 0); // Heuristic will be added here}
}

SearchNode *SearchNode::generateAStarSuccessorNodeIfUnvisited(char action)
{
    GameState *nextState = currentState->generateNextState(action);
    if (visitedSet->isStateVisited(nextState))
    {
        delete nextState;
        return nullptr;
    }
    int heuristicValue = nextState->calculateHeuristic(*boxGoalPositions);
    int transitionCost = 1;
    return new SearchNode(visitedSet, frontier, this, boxGoalPositions, nextState, action, transitionCost, heuristicValue); // Heuristic will be added here
}

bool SearchNode::isActionValid(char action)
{
    return currentState->isActionValid(action);
}

/**
 * Wrapper to populate the map with entities . O(N) cost, where N denotes the number of boxes
 */
void SearchNode::addEntitiesToRawMap()
{
    currentState->updateMapWithState();
}

/**
 * Wrapper to replace the boxes with empty cells . O(N) cost, where N denotes the number of boxes
 */
void SearchNode::removeEntitiesFromUpdatedMap()
{
    currentState->removeEntitiesFromMap();
}