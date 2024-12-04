#pragma once

#include "Life.h"
#include "ConsoleParser.h"
#include <iostream>

namespace LifeGame {
    namespace FileService {
        void setCellFromFile(Life &life, const std::string &line);

        void setSizeFromFile(Life &life, const std::string &line);

        void setRulesFromFile(Life &life, const std::string &line);

        void setNameFromLine(Life &life, const std::string &line);

        void getUniverseFromFile(Life &life, const std::string &path);

        void saveToFile(const Life &life, const std::string &path);
    }

    namespace GameCommandHandler {
        void ProcessCommands(Life &life);
    }

    namespace GameStart {
        void runOffline(ConsoleParser &consoleParser, Life &life);

        void runDefault(Life &life);

        void runInputFile(ConsoleParser &consoleParser, Life &life);
    }
}