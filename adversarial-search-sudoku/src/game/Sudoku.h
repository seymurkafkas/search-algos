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
        GameState getNextState(Action currentAction);

        void undoAction();
        void applyAction();
        std::vector<Action> &getAvailableActions();
        bool isTerminalState();

    private:
        bool isActionValid(Action consideredAction);
        bool checkRowConstraint(Action consideredAction);
        bool checkColumnConstraint(Action consideredAction);
        bool checkGroupConstraint(Action consideredAction);
        void fillAvailableActions();
        int getRow(Coordinates position);
        int getColumn(Coordinates position);
        int getGroup(Coordinates position);

        int **grid;
        std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> unassignedEntries;
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