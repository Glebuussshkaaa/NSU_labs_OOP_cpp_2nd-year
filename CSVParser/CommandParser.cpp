#include "CommandParser.h"
#include <iostream>


std::string CommandParser::getCSVFilePath() const {
    return CSVFilePath;
}

int CommandParser::getNumSkipLines() const {
    return NumSkipLines;
}

[[nodiscard]] bool CommandParser::getHelpMode() const {
    return helpMode;
}

void CommandParser::parseOptions(int argc, const char **argv) {
    if (argc == 1) {
        std::cout << "Invalid parameters. Use -h for help." << std::endl;
        helpMode = true;
        return;
    } else if (argc == 2) {
        if (std::string_view(argv[1]) == "-h") {
            printHelp();
            helpMode = true;
            return;
        }
        CSVFilePath += argv[1];
    } else if (argc == 3) {
        CSVFilePath += argv[1];
        NumSkipLines += std::stoi(argv[2]);
        if (NumSkipLines < 0) {
            throw std::invalid_argument("Negative number of skip lines");
        }
    }
}

void CommandParser::printHelp() {
    std::cout << "\n\t\t\tCSV Parser" << std::endl
              << std::endl
              << "\t1 argument \t--\t csv file" << std::endl
              << "\t2 argument \t--\t skip first lines count (default = 0)" << std::endl
              << std::endl;
}


