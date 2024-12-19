#pragma once

#include <string>
#include <vector>

class ConfigParser {
private:
    std::vector<std::string> converters;
    std::vector<std::string> arguments;

    void readArgMute(const std::string &line, int lineNumber);

    void readArgMix(const std::string &line, int lineNumber);

    void readArgBoost(const std::string &line, int lineNumber);

public:
    ConfigParser() = default;

    void parser(const std::string &configPath);

    [[nodiscard]] std::vector<std::string> getConverters() const;

    [[nodiscard]] std::vector<std::string> getArguments() const;
};
