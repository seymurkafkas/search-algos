#ifndef GAME_CLOSEDSET_H
#define GAME_CLOSEDSET_H

#include <unordered_map>
#include <Gamestate.h>
#include <Hash.h>

class ClosedSet
{
public:
    void markAsVisited(GameState *visitedState);
    bool isStateVisited(GameState *checkedState);

private:
    std::unordered_map<GameState *, bool, GameStateHasher, GameStateEquivalence> mapOfVisitedStates;
};

#endif