#include "Life.h"
#include "ConsoleParser.h"
#include "LifeGame.h"
#include <iostream>

int main(int argc, char **argv) {
    try {
        Life life;
        ConsoleParser consoleParser;

        consoleParser.parseOptions(argc, argv);
        std::string gameMode = consoleParser.getGameMode();

        if (gameMode == "offline") {
            LifeGame::GameStart::runOffline(consoleParser, life);
        } else if (gameMode == "default") {
            LifeGame::GameStart::runDefault(life);
        } else if (gameMode == "input file") {
            LifeGame::GameStart::runInputFile(consoleParser, life);
        }
    } catch (const std::ios_base::failure &e) {
        std::cerr << "File error: " << e.what() << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
        std::cerr << "std::out_of_range: " << e.what() << std::endl;
    }

    return 0;
}