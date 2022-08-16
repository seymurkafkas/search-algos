#ifndef SudokuGameGuard
#define SudokuGameGuard

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Hasher.h"
#include "Benchmark.h"

typedef std::pair<int, int> Coordinates;

namespace Sudoku
{
    struct Action
    {
        Coordinates position;
        int number;
    };

    class GameState
    {
    public:
        GameState();
        ~GameState();
        GameState(std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> unassignedEntries, int **grid, bool areActionsInitialized, Action lastAction);
        GameState(GameState &previous, Action prevAction);
        bool isTerminalState();
        GameState getNextState(Action currentAction);
        bool isActionValid(Action consideredAction);
        bool checkRowConstraint(Action consideredAction);
        bool checkColumnConstraint(Action consideredAction);
        bool checkGroupConstraint(Action consideredAction);
        void fillAvailableActions();
        std::vector<Action> &getAvailableActions();
        int getRow(Coordinates position);
        int getColumn(Coordinates position);
        int getGroup(Coordinates position);
        void applyAction();
        void undoAction();
        std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> unassignedEntries;

    private:
        int **grid;
        std::vector<Action> availableActions;
        bool areActionsInitialized;
        Action previousAction;
    };

    class Game
    {
    public:
        Game();
        Game(std::string);
        void setGrid(int **);
        void setInitialState(GameState initialState);
        void printMap();
        GameState initialState;

    private:
        int **grid;
    };
};

#endif