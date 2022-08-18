
#include <vector>
#include <Actions.h>

std::vector<char> &Actions::getActionSet()
{
    return actions;
}

void Actions::defineActionSet()
{
    Actions::addAction('R');
    Actions::addAction('D');
    Actions::addAction('U');
    Actions::addAction('L');
}

void Actions::addAction(char actionToAdd)
{
    actions.push_back(actionToAdd);
}

std::vector<char> Actions::actions;