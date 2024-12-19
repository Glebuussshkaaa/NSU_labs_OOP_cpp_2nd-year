#pragma once

#include <string>
#include <vector>

class CommandParser {
private:
    std::string configPath = "../filesForWork/configFiles/";
    std::vector<std::string> inputFiles;
    std::string outputFile = "../filesForWork/outputFiles/";
public:
    CommandParser() = default;

    void parser(int argc, const char **argv);

    [[nodiscard]]std::string getConfigPath() const;

    [[nodiscard]]std::vector<std::string> getInputFiles() const;

    [[nodiscard]]std::string getOutputFile() const;

    void printHelp() const;
};