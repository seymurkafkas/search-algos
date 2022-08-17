/**
    @file box-pusher.cpp
    @author Seymur Kafkas
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>
#include <utility>
#include <tuple>
#include <algorithm>
#include <cstdio>
#include <queue>
#include <stack>
#include <utility>
#include <unordered_map>

class SearchNode;
class GameState;

/**
 *      Combine the previous seed with the current key. Adapted from the Boost Hash libraries.
 * @param seed the previous value of the hash
 * @param coordinates The current position pair to be combined
 * @return The updated hash value
 */
std::size_t combineHashValue(std::size_t seed, const std::pair<int, int> &coordinates)
{
    std::hash<int> hasher;
    seed ^= hasher(coordinates.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hasher(coordinates.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

struct NodeComparator
{
    bool operator()(SearchNode *lhs, SearchNode *rhs);
};

class Frontier
{
public:
    virtual bool isEmpty() = 0;
    virtual SearchNode *popNextNode() = 0;
    virtual void addNode(SearchNode *) = 0;

private:
};

class PriorityQueue : public Frontier
{
public:
    bool isEmpty();
    SearchNode *popNextNode();
    void addNode(SearchNode *);

private:
    std::priority_queue<SearchNode *, std::vector<SearchNode *>, NodeComparator> minheap;
};

class Queue : public Frontier
{
public:
    bool isEmpty();
    SearchNode *popNextNode();
    void addNode(SearchNode *);

private:
    std::queue<SearchNode *> nodeQueue;
};

class GameState
{
public:
    GameState();
    // GameState(std::string, std::vector<std::pair<int, int>>, std::pair<int, int>);
    bool isGoalReached(std::vector<std::pair<int, int>> &);
    void updateMapWithState();
    void removeEntitiesFromMap();
    void moveBox(int, std::pair<int, int>);
    void moveAgent(std::pair<int, int>);
    std::string getGridContentAtPosition(std::pair<int, int>);
    GameState *generateNextState(char);
    bool operator==(const GameState &);
    bool isActionValid(char);
    void setGameMap(std::string **);
    void printMap();
    void printState();
    int calculateHeuristic(std::vector<std::pair<int, int>> &);

    std::string **gameMap;
    int rowCount, columnCount;
    std::vector<std::pair<int, int>> boxPositions;
    std::pair<int, int> agentLocation;
};

std::size_t hashGameState(const GameState &stateToHash)
{
    size_t hashValue = 0;
    hashValue = combineHashValue(hashValue, stateToHash.agentLocation);
    for (auto &coordinate : stateToHash.boxPositions)
    {
        hashValue = combineHashValue(hashValue, coordinate);
    }
    return hashValue;
}

struct GameStateEquivalence
{
    bool operator()(GameState *firstState, GameState *secondState) const
    {
        return *firstState == *secondState;
    }
};

struct GameStateHasher
{
    std::size_t operator()(const GameState *hashedState) const
    {
        return hashGameState(*hashedState);
    }
};

class ClosedSet
{
public:
    void markAsVisited(GameState *visitedState)
    {
        mapOfVisitedStates[visitedState] = true;
    }

    bool isStateVisited(GameState *checkedState)
    {
        return mapOfVisitedStates.find(checkedState) != mapOfVisitedStates.end();
    }

private:
    std::unordered_map<GameState *, bool, GameStateHasher, GameStateEquivalence> mapOfVisitedStates;
};

class SearchNode
{
public:
    SearchNode();
    SearchNode(ClosedSet *, Frontier *, SearchNode *, std::vector<std::pair<int, int>> *, GameState *, char, int, int);
    ~SearchNode();
    bool isGoalReached();
    bool operator<(const SearchNode &);
    void expandNode();
    void expandNodeAStar();
    SearchNode *generateSuccessorNode(char, bool);
    SearchNode *generateAStarSuccessorNodeIfUnvisited(char);
    bool isActionValid(char);
    void addEntitiesToRawMap();
    void removeEntitiesFromUpdatedMap();
    void writeSolutionToFile(std::string);

    Frontier *frontier;
    ClosedSet *visitedSet;
    SearchNode *parent;
    char lastTransitionAction;
    std::vector<std::pair<int, int>> *boxGoalPositions;
    int pathCost, totalCost, heuristicCost;
    GameState *currentState;
};

namespace std
{
    std::pair<int, int> operator+(const std::pair<int, int> &a, const std::pair<int, int> &b)
    {
        return std::make_pair(a.first + b.first, a.second + b.second);
    }
}

namespace util
{
    int getBoxNumber(std::string gridContent)
    {
        int i;
        if (sscanf(gridContent.c_str(), "S%d", &i) != 1)
        {
            throw "Invalid Grid Content";
        }
        return i - 1;
    }

    bool isBoxPresent(std::string gridContent)
    {
        if (gridContent.length() < 1 || gridContent[0] != 'S')
        {
            return false;
        }
        return true;
    }

    int getManhattanDistance(std::pair<int, int> firstCoordinate, std::pair<int, int> secondCoordinate)
    {
        int deltaY = firstCoordinate.first - secondCoordinate.first;
        int deltaX = firstCoordinate.second - secondCoordinate.second;
        return abs(deltaX) + abs(deltaY);
    }

}

class Actions
{
public:
    static std::vector<char> &getActionSet()
    {
        return actions;
    }

    static void defineActionSet()
    {
        Actions::addAction('R');
        Actions::addAction('D');
        Actions::addAction('U');
        Actions::addAction('L');
    }

private:
    static void addAction(char actionToAdd)
    {
        actions.push_back(actionToAdd);
    }
    static std::vector<char> actions;
};

std::vector<char> Actions::actions;

enum SearchAlgorithm
{
    iterativeDeepening,
    breadthFirst,
    aStar
};

class BoxPusherGame
{

public:
    BoxPusherGame();
    BoxPusherGame(std::string, std::string);
    void createGameFromFile();
    SearchNode *breadthFirstSearch();
    SearchNode *iterativeDeepeningSearch();
    SearchNode *recursiveDepthLimitedSearch(SearchNode *, int);
    SearchNode *aStarSearch();
    void writeSolutionToFile(SearchNode *);
    void setMapColumnSize(int);
    void setMapRowSize(int);
    int getMapColumnSize();
    int getMapRowSize();
    void setGameMap(std::string **);
    void printGame();
    void searchForSolutionWithAlgorithm(SearchAlgorithm);
    std::string **getGameMap();
    void setInitialGameState(GameState *);

    std::vector<std::pair<int, int>> boxGoalPositions;
    std::string **gameMap;
    std::string outputFileName;
    std::string inputFilePath;
    GameState *initialState;
    int mapColumnSize, mapRowSize;
};

class FileHandler
{

public:
    /**
     *      Initialises the game world and the starting state from the given filepath.
     * @param filePath Path of the read file containing game info.
     * @param game Reference to the game that will be mutated
     */
    static void createGameFromFile(std::string filePath, BoxPusherGame &game)
    {
        std::fstream inputTextFileHandler;
        inputTextFileHandler.open(filePath);
        validateFileHandler(inputTextFileHandler);
        int rowCount, columnCount;
        inputTextFileHandler >> rowCount >> columnCount;
        game.setMapRowSize(rowCount);
        game.setMapColumnSize(columnCount);
        std::string **gameMap = createMapWithDimensions(rowCount, columnCount);
        game.setGameMap(gameMap);
        fillGameFromFileHandler(game, inputTextFileHandler);
    }

    /**
     *      Writes the action sequence that leads to the node in the file.
     * @param currentNode Search node to be traversed in the reverse order
     * @param fileName Name of the output file with printed steps.
     */
    static void writeStepsToFile(SearchNode *currentNode, std::string fileName)
    {
        std::fstream outputTextFileHandler;
        outputTextFileHandler.open(fileName, std::fstream::out);
        validateFileHandler(outputTextFileHandler);
        outputTextFileHandler << currentNode->pathCost << std::endl;
        std::stack<char> actionSequence;
        for (SearchNode *iterator = currentNode; iterator->parent != nullptr; iterator = iterator->parent)
        {
            actionSequence.push(iterator->lastTransitionAction);
        }
        while (!actionSequence.empty())
        {
            char currentAction = actionSequence.top();
            actionSequence.pop();
            outputTextFileHandler << currentAction << std::endl;
        }
    }

    static void deleteGameMap(std::string **gameMap, int rowCount)
    {
        for (int i = 0; i < rowCount; i++)
        {
            delete[] gameMap[i];
        }
        delete[] gameMap;
    }

    static void printMap(std::string **gameMap, int rowCount, int columnCount)
    {
        for (int i = 0; i < rowCount; ++i)
        {
            for (int j = 0; j < columnCount; ++j)
            {
                std::cout << gameMap[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    static bool isNextInputValid(std::fstream &inputFileToValidate)
    {
        return inputFileToValidate.good();
    }

    static void validateFileHandler(std::fstream &fileHandlerToValidate)
    {
        if (fileHandlerToValidate.fail())
        {
            throw std::string("Invalid FilePath\n");
        }
    }

    static std::string **createMapWithDimensions(int rowCount, int columnCount)
    {
        std::string **gameMap = new std::string *[rowCount];
        for (int i = 0; i < rowCount; i++)
        {
            gameMap[i] = new std::string[columnCount];
        }
        return gameMap;
    }

    /**
     *
     * @param game Reference to the game instance that will be mutated.
     * @param inputTextFileHandler Reference to the read file stream with game info.
     */
    static void fillGameFromFileHandler(BoxPusherGame &game, std::fstream &inputTextFileHandler)
    {
        std::vector<std::tuple<std::string, int, int>> boxGoalLocations;
        std::vector<std::tuple<std::string, int, int>> boxLocations;
        std::pair<int, int> agentLocation;
        std::string **gameMap = game.getGameMap();
        GameState *initialGameState = new GameState;
        for (int i = 0; i < game.getMapRowSize(); ++i)
        {
            for (int j = 0; j < game.getMapColumnSize(); ++j)
            {
                if (!isNextInputValid(inputTextFileHandler))
                {
                    // deleteGameMap(gameMap, rowCount);
                    throw std::string("Invalid File Content\n");
                }
                std::string gridContent;
                inputTextFileHandler >> gridContent;
                gameMap[i][j] = gridContent;

                if (!(gridContent == "W" || gridContent == "E"))
                {
                    if (gridContent == "A")
                    {
                        gameMap[i][j] = "E";
                        agentLocation = std::make_pair(i, j);
                    }
                    else if (gridContent[0] == 'S')
                    {
                        boxLocations.push_back(std::make_tuple(gridContent, i, j));
                    }
                    else if (gridContent[0] == 'G')
                    {
                        gameMap[i][j] = "E";
                        boxGoalLocations.push_back(std::make_tuple(gridContent, i, j));
                    }
                }
            }
        }
        std::sort(boxLocations.begin(), boxLocations.end());
        std::sort(boxGoalLocations.begin(), boxGoalLocations.end());
        initialGameState->agentLocation = agentLocation;
        for (unsigned int i = 0; i < boxLocations.size(); i++)
        {
            initialGameState->boxPositions.push_back(std::make_pair(std::get<1>(boxLocations[i]), std::get<2>(boxLocations[i])));
            game.boxGoalPositions.push_back(std::make_pair(std::get<1>(boxGoalLocations[i]), std::get<2>(boxGoalLocations[i])));
        }
        initialGameState->setGameMap(gameMap);
        initialGameState->columnCount = game.getMapColumnSize();
        initialGameState->rowCount = game.getMapRowSize();

        game.setInitialGameState(initialGameState);
    }
};

class BenchmarkTimer
{
public:
    BenchmarkTimer(std::string benchmarkDescriptor = "")
    {
        startTimePoint = std::chrono::high_resolution_clock::now();
        customBenchmarkDescription = benchmarkDescriptor;
    }

    ~BenchmarkTimer()
    {
        endTimePoint = std::chrono::high_resolution_clock::now();
        long long runtimeInBlockInMicroSeconds = calculateRuntimeInMicroSeconds();
        displayRuntime(runtimeInBlockInMicroSeconds);
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    std::string customBenchmarkDescription;

    long long calculateRuntimeInMicroSeconds()
    {
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
        auto runtimeInMicroSeconds = end - start;
        return runtimeInMicroSeconds;
    }

    void displayRuntime(long long runtimeInMicroSeconds)
    {
        std::cout << "The block " << (customBenchmarkDescription == "" ? "" : "for ") << customBenchmarkDescription << " ran for " << runtimeInMicroSeconds << " microseconds." << std::endl;
    }
};

class Benchmark
{
public:
    static void increaseNodeCountInMemory()
    {
        currentNodeCountInMemory++;
        totalGeneratedNodeCount++;
        if (currentNodeCountInMemory > maximumNodeCountInMemory)
        {
            maximumNodeCountInMemory = currentNodeCountInMemory;
        }
    }

    static void decreaseNodeCountInMemory()
    {
        currentNodeCountInMemory--;
    }

    static void increaseNumberOfNodeExpansions()
    {
        numberOfNodeExpansions++;
    }

    static void initialiseNodeBookkeeper()
    {
        totalGeneratedNodeCount = 0;
        currentNodeCountInMemory = 0;
        maximumNodeCountInMemory = 0;
        numberOfNodeExpansions = 0;
    }

    static void printResults()
    {

        std::cout << "Total count of generated nodes:" << totalGeneratedNodeCount << std::endl;
        std::cout << "The maximum number of nodes in memory:" << maximumNodeCountInMemory << std::endl;
        std::cout << "The number of node expansions:" << numberOfNodeExpansions << std::endl;
    }

private:
    static int totalGeneratedNodeCount;
    static int currentNodeCountInMemory;
    static int maximumNodeCountInMemory;
    static int numberOfNodeExpansions;
};

int Benchmark::totalGeneratedNodeCount;
int Benchmark::currentNodeCountInMemory;
int Benchmark::maximumNodeCountInMemory;
int Benchmark::numberOfNodeExpansions;

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
    {
        gameMap[position.first][position.second] = "E";
    }
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
    {
        return true;
    }

    else if (contentOfNextPosition == "W")
    {
        return false;
    }

    else if (contentOfNextPosition[0] == 'S')
    {
        std::string contentOfOneFurtherPosition = getGridContentAtPosition(newPositionForAgent + changeInPosition);
        return contentOfOneFurtherPosition != "W" && contentOfOneFurtherPosition[0] != 'S';
    }

    else
    {
        return true;
    }
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

bool NodeComparator::operator()(SearchNode *lhs, SearchNode *rhs)
{
    return *lhs < *rhs;
}

std::pair<std::string, std::string> fetchCommandLineArguments(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        throw std::string("Invalid Number of Command Line Arguments\n");
    }
    else
    {
        std::string outputFileName = argc == 3 ? argv[2] : "output.txt";
        std::string inputFilePath = argv[1];
        return std::make_pair(inputFilePath, outputFileName);
    }
}

int main(int argc, char **argv)
{
    try
    {
        auto iOFilePair = fetchCommandLineArguments(argc, argv);
        std::string inputFilePath = iOFilePair.first;
        std::string outputFileName = iOFilePair.second;
        BoxPusherGame gameInstance(inputFilePath, outputFileName);
        Actions::defineActionSet();
        gameInstance.createGameFromFile();
        Benchmark::initialiseNodeBookkeeper();
        { // Timer prints the execution time of code within the block
            BenchmarkTimer timerForBlock("Main Algorithm");
            gameInstance.searchForSolutionWithAlgorithm(aStar); // breadthFirst, iterativeDeepening, aStar
        }
        Benchmark::printResults();
    }
    catch (std::string err)
    {
        std::cout << err;
    }
    return 0;
}