#ifndef GAME_SEARCHNODE_H
#define GAME_SEARCHNODE_H

#include <ClosedSet.h>
#include <Frontier.h>

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

#endif