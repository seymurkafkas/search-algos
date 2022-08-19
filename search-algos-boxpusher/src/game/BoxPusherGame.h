#ifndef GAME_BOXPUSHERGAME_H
#define GAME_BOXPUSHERGAME_H

#include <SearchNode.h>
#include <GameState.h>
#include <SearchAlgorithm.h>
#include <string>

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

#endif