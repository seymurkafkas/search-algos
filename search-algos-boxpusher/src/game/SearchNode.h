#ifndef GAME_SEARCHNODE_H
#define GAME_SEARCHNODE_H

#include <vector>
#include <utility>
#include <string>

class ClosedSet;
class Frontier;
class GameState;

class SearchNode
{
    friend class BoxPusherGame;
    friend class FileHandler;

public:
    SearchNode();
    SearchNode(ClosedSet *, Frontier *, SearchNode *, std::vector<std::pair<int, int>> *, GameState *, char, int, int);
    ~SearchNode();
    bool operator<(const SearchNode &);

private:
    void expandNode();
    void expandNodeAStar();
    bool isGoalReached();
    void removeEntitiesFromUpdatedMap();
    void writeSolutionToFile(std::string);
    SearchNode *generateSuccessorNode(char, bool);
    SearchNode *generateAStarSuccessorNodeIfUnvisited(char);
    bool isActionValid(char);
    void addEntitiesToRawMap();

    Frontier *frontier;
    ClosedSet *visitedSet;
    SearchNode *parent;
    char lastTransitionAction;
    std::vector<std::pair<int, int>> *boxGoalPositions;
    int pathCost, totalCost, heuristicCost;
    GameState *currentState;
};

struct NodeComparator
{
    bool operator()(SearchNode *lhs, SearchNode *rhs);
};

#endif