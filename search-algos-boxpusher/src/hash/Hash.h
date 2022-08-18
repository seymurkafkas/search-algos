#ifndef HASH_HASH_H
#define HASH_HASH_H

#include <iostream>
#include <GameState.h>
#include <SearchNode.h>

struct GameStateHasher
{
    std::size_t operator()(const GameState *hashedState) const
    {
        return hashGameState(*hashedState);
    }
};

struct NodeComparator
{
    bool operator()(SearchNode *lhs, SearchNode *rhs);
};

bool NodeComparator::operator()(SearchNode *lhs, SearchNode *rhs)
{
    return *lhs < *rhs;
}

struct GameStateEquivalence
{
    bool operator()(GameState *firstState, GameState *secondState) const
    {
        return *firstState == *secondState;
    }
};

std::size_t combineHashValue(std::size_t seed, const std::pair<int, int> &coordinates);

std::size_t hashGameState(const GameState &stateToHash);

#endif