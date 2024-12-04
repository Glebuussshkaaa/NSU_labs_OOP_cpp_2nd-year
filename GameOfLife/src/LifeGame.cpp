#include "Life.h"
#include "ConsoleParser.h"
#include <sstream>
#include <fstream>
#include <iostream>


namespace LifeGame {
    namespace FileService {
        void setCellFromFile(Life &life, const std::string &line) {
            std::istringstream stream(line);
            int x, y;
            if (stream >> x >> y) {
                life.addCell(x, y);
            } else {
                throw std::invalid_argument("Invalid cell format: " + line);
            }
        }

        void setSizeFromFile(Life &life, const std::string &line) {
            size_t slashPos = line.find('/');
            if (slashPos != std::string::npos) {
                life.setBoardWidth(std::stoi(line.substr(3, slashPos - 3)));
                life.setBoardHeight(std::stoi(line.substr(slashPos + 1)));
            } else {
                throw std::invalid_argument("Invalid size format: " + line);
            }
        }

        void setRulesFromFile(Life &life, const std::string &line) {
            size_t posB = line.find('B');
            size_t posS = line.find('S');

            if (posB != std::string::npos && posS != std::string::npos) {
                for (size_t i = posB + 1; i < posS; ++i) {
                    if (isdigit(line[i])) {
                        life.addBirthRule(line[i] - '0');
                    }
                }

                for (size_t i = posS + 1; i < line.size(); ++i) {
                    if (isdigit(line[i])) {
                        life.addSurvivalRule(line[i] - '0');
                    }
                }
            } else {
                throw std::invalid_argument("Invalid rule format: " + line);
            }
        }

        void setNameFromLine(Life &life, const std::string &line) {
            if (line.size() > 3) {
                life.setUniverseName(line.substr(3));
            } else {
                throw std::invalid_argument("Invalid name format: " + line);
            }
        }

        void getUniverseFromFile(Life &life, const std::string &path) {
            std::ifstream inputFile(path);
            if (!inputFile) {
                throw std::ios_base::failure("Failed to open file: " + path);
            }

            std::string line;
            while (std::getline(inputFile, line)) {
                if (line.rfind("#N ", 0) == 0) {
                    setNameFromLine(life, line);
                } else if (line.rfind("#S ", 0) == 0) {
                    setSizeFromFile(life, line);
                } else if (line.rfind("#R ", 0) == 0) {
                    setRulesFromFile(life, line);
                } else if (line.rfind("#Life", 0) == 0) {
                    continue;
                } else if (!line.empty() && line[0] != '#') {
                    setCellFromFile(life, line);
                }
            }

            inputFile.close();
        }

        void saveToFile(const Life &life, const std::string &path) {
            std::ofstream outputFile(path);
            if (!outputFile) {
                throw std::ios_base::failure("Failed to open file: " + path);
            }

            outputFile << "#Life 1.06" << std::endl;
            outputFile << "#N " << life.getUniverseName() << std::endl;

            outputFile << "#R B";
            for (int birth: life.getBirthRules()) {
                outputFile << birth;
            }
            outputFile << "/S";
            for (int survival: life.getSurvivalRules()) {
                outputFile << survival;
            }
            outputFile << std::endl;

            outputFile << "#S " << life.getBoardWidth() << "/" << life.getBoardHeight() << std::endl;

            for (int x = 0; x < life.getBoardWidth(); ++x) {
                for (int y = 0; y < life.getBoardHeight(); ++y) {
                    if (life.cellExists(x, y)) {
                        outputFile << x << " " << y << std::endl;
                    }
                }
            }

            outputFile.close();
        }
    }

    namespace GameCommandHandler {
        void ProcessCommands(Life &life) {
            while (true) {
                std::string command;
                std::getline(std::cin, command);

                if (command == "exit") {
                    break;
                } else if (command == "help") {
                    std::cout << "dump <filename> - save universe to file;\n"
                                 "tick <n> - calculate n (default 1) iterations and print result;\n"
                                 "exit - finish game;\n"
                                 "help - print help about commands.\n";
                } else if (command.rfind("dump ", 0) == 0) {
                    std::string path = "../examples/";

                    std::string filename = command.substr(5);
                    if (filename.empty()) {
                        std::cout << "help - dump command requires a filename argument.\n";
                        continue;
                    }

                    path += filename;

                    std::ifstream file(path);
                    if (!file) {
                        std::cout << "help - The file " << filename << " does not exist in the examples directory.\n";
                        continue;
                    }
                    file.close();

                    LifeGame::FileService::saveToFile(life, path);
                } else if (command.rfind("tick", 0) == 0) {
                    int countIterations = 1;

                    if (command.size() > 4) {
                        std::string iterationsStr = command.substr(5);
                        try {
                            countIterations = std::stoi(iterationsStr);
                        } catch (const std::invalid_argument&) {
                            std::cout << "help - tick <n> - calculate n (default 1) iterations and print result (e.g., tick 5, tick).\n";
                            continue;
                        }
                    }

                    for (int i = 0; i < countIterations; ++i) {
                        life.advanceToNextGeneration();
                    }

                    life.printUniverse();
                } else if (!command.empty()) {
                    std::cout << "help - Unknown command. Available commands:\n"
                                 "dump <filename> - save universe to file;\n"
                                 "tick <n> - calculate n (default 1) iterations and print result;\n"
                                 "exit - finish game;\n"
                                 "help - print help about commands.\n";
                    continue;
                }
            }
        }
    }

    namespace GameStart {
        void runOffline(ConsoleParser &consoleParser, Life &life) {
            std::string path = "../examples/";
            path += consoleParser.getInputFile();
            FileService::getUniverseFromFile(life, path);

            for (int i = 0; i < consoleParser.getCountOfIterations(); ++i) {
                life.advanceToNextGeneration();
            }

            path = "../examples/";
            path += consoleParser.getOutputFile();
            FileService::saveToFile(life, path);
        }

        void runDefault(Life &life) {
            FileService::getUniverseFromFile(life, "../examples/input.life");
            life.printUniverse();
            GameCommandHandler::ProcessCommands(life);
        }

        void runInputFile(ConsoleParser &consoleParser, Life &life) {
            std::string path = "../examples/";
            path += consoleParser.getInputFile();
            FileService::getUniverseFromFile(life, path);
            life.printUniverse();
            GameCommandHandler::ProcessCommands(life);
        }
    }
}