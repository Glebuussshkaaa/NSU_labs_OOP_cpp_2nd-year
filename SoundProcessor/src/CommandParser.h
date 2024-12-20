#pragma once

#include <string>
#include <vector>

class CommandParser {
private:
    std::string configPath = "../filesForWork/configFiles/";
    std::vector<std::string> inputFilesPaths;
    std::string outputFilePath = "../filesForWork/outputFiles/";
    bool helpMode = false;
public:
    CommandParser() = default;

    void parser(int argc, const char **argv);

    [[nodiscard]]std::string getConfigPath() const;

    [[nodiscard]]std::vector<std::string> getInputFilesPaths() const;

    [[nodiscard]]std::string getOutputFilePath() const;

    [[nodiscard]] bool getHelpMode() const;

    static void printHelp();
};