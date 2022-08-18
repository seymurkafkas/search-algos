#include <GameState.h>
#include <string>
#include <iostream>
#include <util.h>

GameState::GameState()
{
}

bool GameState::isGoalReached(std::vector<std::pair<int, int>> &boxGoalPositions)
{
    return boxGoalPositions == boxPositions;
}

std::string GameState::getGridContentAtPosition(std::pair<int, int> position)
{
    return gameMap[position.first][position.second];
}

/**
 * Populate the map with entities . O(N) cost, where N denotes the number of boxes
 */
void GameState::updateMapWithState()
{
    int currentIndex = 0;
    for (auto position : boxPositions)
    {
        gameMap[position.first][position.second] = "S" + std::to_string(currentIndex + 1);
        currentIndex++;
    }
    gameMap[agentLocation.first][agentLocation.second] = "A";
}

/**
 * Replace the boxes with empty cells . O(N) cost, where N denotes the number of boxes
 */
void GameState::removeEntitiesFromMap()
{
    for (auto position : boxPositions)
        gameMap[position.first][position.second] = "E";
    gameMap[agentLocation.first][agentLocation.second] = "E";
}

bool GameState::operator==(const GameState &rhs)
{
    bool agentLocationMatches = this->agentLocation == rhs.agentLocation;
    bool boxLocationMatches = this->boxPositions == rhs.boxPositions;
    return agentLocationMatches && boxLocationMatches;
}

bool GameState::isActionValid(char action)
{
    int deltaRow = 0, deltaColumn = 0;
    switch (action)
    {
    case 'U':
        deltaRow = -1;
        break;
    case 'D':
        deltaRow = 1;
        break;
    case 'L':
        deltaColumn = -1;
        break;
    case 'R':
        deltaColumn = 1;
        break;
    }
    auto changeInPosition = std::make_pair(deltaRow, deltaColumn);
    auto newPositionForAgent = changeInPosition + agentLocation;
    std::string contentOfNextPosition = getGridContentAtPosition(newPositionForAgent);

    if (contentOfNextPosition == "E")
        return true;
    else if (contentOfNextPosition == "W")
        return false;
    else if (contentOfNextPosition[0] == 'S')
    {
        std::string contentOfOneFurtherPosition = getGridContentAtPosition(newPositionForAgent + changeInPosition);
        return contentOfOneFurtherPosition != "W" && contentOfOneFurtherPosition[0] != 'S';
    }
    else
        return true;
}

void GameState::setGameMap(std::string **gameMapInput)
{
    gameMap = gameMapInput;
}

GameState *GameState::generateNextState(char action)
{
    GameState *nextState = new GameState(*this);
    int deltaRow = 0, deltaColumn = 0;
    switch (action)
    {
    case 'U':
        deltaRow = -1;
        break;
    case 'D':
        deltaRow = 1;
        break;
    case 'L':
        deltaColumn = -1;
        break;
    case 'R':
        deltaColumn = 1;
        break;
    }
    auto changeInPosition = std::make_pair(deltaRow, deltaColumn);
    auto newPositionForAgent = changeInPosition + agentLocation;
    std::string contentOfNextPosition = getGridContentAtPosition(newPositionForAgent);
    if (util::isBoxPresent(contentOfNextPosition))
    {
        int indexOfBox = util::getBoxNumber(contentOfNextPosition);
        nextState->moveBox(indexOfBox, changeInPosition);
    }
    nextState->moveAgent(changeInPosition);
    return nextState;
}

void GameState::moveAgent(std::pair<int, int> changeInPosition)
{
    agentLocation = changeInPosition + agentLocation;
}

void GameState::moveBox(int boxIndex, std::pair<int, int> changeInPosition)
{
    boxPositions[boxIndex] = boxPositions[boxIndex] + changeInPosition; // Need to overload +=
}

void GameState::printMap()
{
    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
            std::cout << gameMap[i][j] << " ";

        std::cout << std::endl;
    }

    std::cout << "===================================\n";
    std::cout << "===================================\n\n";
}

void GameState::printState()
{
    for (unsigned int i = 0; i < this->boxPositions.size(); i++)
        std::cout << "Box " << i << " Location: " << boxPositions[i].first << " , " << boxPositions[i].second << std::endl;
    std::cout << "Agent Location: " << agentLocation.first << " , " << agentLocation.second << std::endl;
}

/**
 *
 * Calculates and returns the heuristic value for the given state. This heuristic is admissible and consistent.
 *
 * @param boxGoalPositions Vector containing goal positions for each box
 * @return Approximate cost to the goal from this state
 */

int GameState::calculateHeuristic(std::vector<std::pair<int, int>> &boxGoalPositions)
{
    int heuristicResult = 0;
    int index = 0;

    for (auto &boxCoordinates : boxPositions)
    {
        heuristicResult += util::getManhattanDistance(boxCoordinates, boxGoalPositions[index]);
        index++;
    }

    return heuristicResult;
}

bool GameStateEquivalence::operator()(GameState *firstState, GameState *secondState) const
{
    return *firstState == *secondState;
}