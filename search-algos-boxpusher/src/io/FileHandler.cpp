#include <FileHandler.h>
#include <stack>
#include <tuple>
#include <iostream>
#include <vector>
#include <GameState.h>
#include <algorithm>

void FileHandler::createGameFromFile(std::string filePath, BoxPusherGame &game)
{
    std::fstream inputTextFileHandler;
    inputTextFileHandler.open(filePath);
    validateFileHandler(inputTextFileHandler);
    int rowCount, columnCount;
    inputTextFileHandler >> rowCount >> columnCount;
    game.setMapRowSize(rowCount);
    game.setMapColumnSize(columnCount);
    std::string **gameMap = createMapWithDimensions(rowCount, columnCount);
    game.setGameMap(gameMap);
    fillGameFromFileHandler(game, inputTextFileHandler);
}

void FileHandler::writeStepsToFile(SearchNode *currentNode, std::string fileName)
{
    std::fstream outputTextFileHandler;
    outputTextFileHandler.open(fileName, std::fstream::out);
    validateFileHandler(outputTextFileHandler);
    outputTextFileHandler << currentNode->pathCost << std::endl;
    std::stack<char> actionSequence;
    for (SearchNode *iterator = currentNode; iterator->parent != nullptr; iterator = iterator->parent)
    {
        actionSequence.push(iterator->lastTransitionAction);
    }
    while (!actionSequence.empty())
    {
        char currentAction = actionSequence.top();
        actionSequence.pop();
        outputTextFileHandler << currentAction << std::endl;
    }
}

void FileHandler::deleteGameMap(std::string **gameMap, int rowCount)
{
    for (int i = 0; i < rowCount; i++)
    {
        delete[] gameMap[i];
    }
    delete[] gameMap;
}

void FileHandler::printMap(std::string **gameMap, int rowCount, int columnCount)
{
    for (int i = 0; i < rowCount; ++i)
    {
        for (int j = 0; j < columnCount; ++j)
        {
            std::cout << gameMap[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
bool FileHandler::isNextInputValid(std::fstream &inputFileToValidate)
{
    return inputFileToValidate.good();
}

void FileHandler::validateFileHandler(std::fstream &fileHandlerToValidate)
{
    if (fileHandlerToValidate.fail())
    {
        throw std::string("Invalid FilePath\n");
    }
}

std::string **FileHandler::createMapWithDimensions(int rowCount, int columnCount)
{
    std::string **gameMap = new std::string *[rowCount];
    for (int i = 0; i < rowCount; i++)
    {
        gameMap[i] = new std::string[columnCount];
    }
    return gameMap;
}

/**
 *
 * @param game Reference to the game instance that will be mutated.
 * @param inputTextFileHandler Reference to the read file stream with game info.
 */
void FileHandler::fillGameFromFileHandler(BoxPusherGame &game, std::fstream &inputTextFileHandler)
{
    std::vector<std::tuple<std::string, int, int>> boxGoalLocations;
    std::vector<std::tuple<std::string, int, int>> boxLocations;
    std::pair<int, int> agentLocation;
    std::string **gameMap = game.getGameMap();
    GameState *initialGameState = new GameState;
    for (int i = 0; i < game.getMapRowSize(); ++i)
    {
        for (int j = 0; j < game.getMapColumnSize(); ++j)
        {
            if (!isNextInputValid(inputTextFileHandler))
            {
                throw std::string("Invalid File Content\n");
            }
            std::string gridContent;
            inputTextFileHandler >> gridContent;
            gameMap[i][j] = gridContent;

            if (!(gridContent == "W" || gridContent == "E"))
            {
                if (gridContent == "A")
                {
                    gameMap[i][j] = "E";
                    agentLocation = std::make_pair(i, j);
                }
                else if (gridContent[0] == 'S')
                {
                    boxLocations.push_back(std::make_tuple(gridContent, i, j));
                }
                else if (gridContent[0] == 'G')
                {
                    gameMap[i][j] = "E";
                    boxGoalLocations.push_back(std::make_tuple(gridContent, i, j));
                }
            }
        }
    }
    std::sort(boxLocations.begin(), boxLocations.end());
    std::sort(boxGoalLocations.begin(), boxGoalLocations.end());
    initialGameState->agentLocation = agentLocation;
    for (unsigned int i = 0; i < boxLocations.size(); i++)
    {
        initialGameState->boxPositions.push_back(std::make_pair(std::get<1>(boxLocations[i]), std::get<2>(boxLocations[i])));
        game.boxGoalPositions.push_back(std::make_pair(std::get<1>(boxGoalLocations[i]), std::get<2>(boxGoalLocations[i])));
    }
    initialGameState->setGameMap(gameMap);
    initialGameState->columnCount = game.getMapColumnSize();
    initialGameState->rowCount = game.getMapRowSize();

    game.setInitialGameState(initialGameState);
}