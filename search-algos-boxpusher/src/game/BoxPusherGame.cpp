#include <BoxPusherGame.h>
#include <string>
#include <FileHandler.h>
#include <ClosedSet.h>

BoxPusherGame::BoxPusherGame()
{
}

BoxPusherGame::BoxPusherGame(std::string inputFilePath, std::string outputFileName) : outputFileName(outputFileName), inputFilePath(inputFilePath)
{
}

void BoxPusherGame::createGameFromFile()
{
    FileHandler::createGameFromFile(inputFilePath, *this);
}

/**
 *
 * Applies BFS to find the shallowest solution. Uses a closed set and a queue for the frontier.
 * @return Pointer to the solution node
 */
SearchNode *BoxPusherGame::breadthFirstSearch()
{
    Queue frontier;
    ClosedSet visitedSet;
    SearchNode *initialNode = new SearchNode(&visitedSet, &frontier, nullptr, &boxGoalPositions, initialState, 'X', 0, 0);
    frontier.addNode(initialNode);
    visitedSet.markAsVisited(initialState);
    SearchNode *current;

    while (!frontier.isEmpty())
    {
        current = frontier.popNextNode();
        if (current->isGoalReached())
            return current;
        else
            current->expandNode();
    }

    return nullptr;
}

/**
 *
 * Applies recursive DLS with increasing depth limits until a solution is found.
 * @return Pointer to the solution node
 */
SearchNode *BoxPusherGame::iterativeDeepeningSearch()
{
    SearchNode *result = nullptr;
    for (int depth = 0; depth < 50; depth++)
    {
        GameState *copyOfInitialState = new GameState(*initialState);
        SearchNode *initialNode = new SearchNode(nullptr, nullptr, nullptr, &boxGoalPositions, copyOfInitialState, 'X', 0, 0);
        result = recursiveDepthLimitedSearch(initialNode, depth);
        if (result)
            break;
    }
    return result;
}

/**
 *
 * Recursively test and expand the node within the depth limit.
 * @param algorithmName Identifier of the search algorithm
 * @return Pointer to the solution node
 */
void BoxPusherGame::searchForSolutionWithAlgorithm(SearchAlgorithm algorithmName)
{
    SearchNode *solution = nullptr;
    switch (algorithmName)
    {
    case SearchAlgorithm::breadthFirst:
        solution = breadthFirstSearch();
        break;
    case SearchAlgorithm::iterativeDeepening:
        solution = iterativeDeepeningSearch();
        break;
    case SearchAlgorithm::aStar:
        solution = aStarSearch();
        break;
    }
    if (solution)
        writeSolutionToFile(solution);
    else
        std::cout << "No solution is found" << std::endl;
}

/**
 *
 * Recursively test and expand the node within the depth limit.
 * @param currentNode The node to be evaluated and expanded.
 * @param depthLimit Maximum permissible depth of search
 * @return Pointer to the solution node
 */
SearchNode *BoxPusherGame::recursiveDepthLimitedSearch(SearchNode *currentNode, int depthLimit)
{
    bool isSolutionFound = currentNode->isGoalReached();
    if (!isSolutionFound && currentNode->pathCost < depthLimit)
    {
        bool isNodeExpansionCountIncreased = false; // Purely for benchmarking
        for (char &consideredAction : Actions::getActionSet())
        {
            currentNode->addEntitiesToRawMap();
            if (currentNode->isActionValid(consideredAction))
            {
                SearchNode *nextNode = currentNode->generateSuccessorNode(consideredAction, false);
                if (!isNodeExpansionCountIncreased)
                {
                    Benchmark::increaseNumberOfNodeExpansions();
                    isNodeExpansionCountIncreased = true;
                }
                currentNode->removeEntitiesFromUpdatedMap();
                SearchNode *result = recursiveDepthLimitedSearch(nextNode, depthLimit);
                if (result)
                    return result;
            }
        }
        currentNode->removeEntitiesFromUpdatedMap();
    }
    else if (isSolutionFound)
    {
        return currentNode;
    }
    delete currentNode;
    return nullptr;
}

/**
 *
 * Calculates and returns the solution node iff it is reachable from the initial node.
 * Uses the sum of Manhattan distances as a heuristic.
 *
 * @return Pointer to the solution node
 */
SearchNode *BoxPusherGame::aStarSearch()
{
    PriorityQueue frontier;
    ClosedSet visitedSet;
    int initialHeuristic = initialState->calculateHeuristic(boxGoalPositions);
    SearchNode *initialNode = new SearchNode(&visitedSet, &frontier, nullptr, &boxGoalPositions, initialState, 'X', 0, initialHeuristic);
    frontier.addNode(initialNode);
    SearchNode *current;
    while (!frontier.isEmpty())
    {
        current = frontier.popNextNode();
        if (visitedSet.isStateVisited(current->currentState))
        {
            delete current;
            continue;
        }
        else
            visitedSet.markAsVisited(current->currentState);

        if (current->isGoalReached())
            return current;
        else
            current->expandNodeAStar();
    }

    return nullptr;
}

void BoxPusherGame::writeSolutionToFile(SearchNode *solutionNode)
{
    solutionNode->writeSolutionToFile(outputFileName);
}

void BoxPusherGame::setMapColumnSize(int mapColumnSizeInput)
{
    mapColumnSize = mapColumnSizeInput;
}

void BoxPusherGame::setMapRowSize(int mapRowSizeInput) { mapRowSize = mapRowSizeInput; }

int BoxPusherGame::getMapColumnSize() { return mapColumnSize; }

int BoxPusherGame::getMapRowSize() { return mapRowSize; }

void BoxPusherGame::setGameMap(std::string **gameMapInput)
{
    gameMap = gameMapInput;
}

/**
 * Print the initial conditions of the game
 */
void BoxPusherGame::printGame()
{
    for (unsigned int i = 0; i < this->boxGoalPositions.size(); i++)
    {
        std::cout << "Box Location:" << initialState->boxPositions[i].first << " , " << initialState->boxPositions[i].second << std::endl;
        std::cout << "Box Goal Location:" << boxGoalPositions[i].first << " , " << boxGoalPositions[i].second << std::endl;
    }

    std::cout << "Agent Location" << initialState->agentLocation.first << " , " << initialState->agentLocation.second << std::endl;
}

std::string **BoxPusherGame::getGameMap()
{
    return gameMap;
}

void BoxPusherGame::setInitialGameState(GameState *inputGameState)
{
    initialState = inputGameState;
}