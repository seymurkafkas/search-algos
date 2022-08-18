#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

#include <vector>
#include <utility>
#include <string>

class GameState
{
public:
    GameState();
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

struct GameStateEquivalence
{
    bool operator()(GameState *firstState, GameState *secondState) const;
};

#endif