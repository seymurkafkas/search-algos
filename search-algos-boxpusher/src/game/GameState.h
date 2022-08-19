#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

#include <vector>
#include <utility>
#include <string>

class GameState
{
    friend class SearchNode;

public:
    GameState();
    bool operator==(const GameState &);
    int calculateHeuristic(std::vector<std::pair<int, int>> &);
    void setGameMap(std::string **);

    std::pair<int, int> agentLocation;
    std::vector<std::pair<int, int>> boxPositions;
    int rowCount, columnCount;

private:
    bool isGoalReached(std::vector<std::pair<int, int>> &);
    void removeEntitiesFromMap();
    void moveBox(int, std::pair<int, int>);
    void moveAgent(std::pair<int, int>);
    std::string getGridContentAtPosition(std::pair<int, int>);
    GameState *generateNextState(char);
    void printMap();
    void printState();
    bool isActionValid(char);
    void updateMapWithState();
    std::string **gameMap;
};

struct GameStateEquivalence
{
    bool operator()(GameState *firstState, GameState *secondState) const;
};

#endif