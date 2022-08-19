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

    void searchForSolutionWithAlgorithm(SearchAlgorithm);
    void createGameFromFile();

    void setInitialGameState(GameState *);
    int getMapRowSize();
    void setMapRowSize(int);
    int getMapColumnSize();
    void setMapColumnSize(int);
    std::string **getGameMap();
    void setGameMap(std::string **);

    std::vector<std::pair<int, int>> boxGoalPositions;

private:
    SearchNode *breadthFirstSearch();
    SearchNode *iterativeDeepeningSearch();
    SearchNode *recursiveDepthLimitedSearch(SearchNode *, int);
    SearchNode *aStarSearch();
    void writeSolutionToFile(SearchNode *);
    void printGame();

    std::string **gameMap;
    std::string outputFileName;
    std::string inputFilePath;
    GameState *initialState;
    int mapColumnSize, mapRowSize;
};

#endif