#pragma once

#include "Cell.h"
#include <set>

class Life {
private:
    std::string universeName;
    int boardWidth = 0;
    int boardHeight = 0;
    std::set<int> survivalRules;
    std::set<int> birthRules;
    std::set<Cell> liveCells;

    [[nodiscard]] int countLivingNeighbours(int x, int y) const;

public:
    Life() = default;

    void advanceToNextGeneration();

    [[nodiscard]] bool cellExists(const int &x, const int &y) const;

    void printUniverse() const;

    [[nodiscard]] std::string getUniverseName() const;

    [[nodiscard]] int getBoardWidth() const;

    [[nodiscard]] int getBoardHeight() const;

    [[nodiscard]] std::set<int> getSurvivalRules() const;

    [[nodiscard]] std::set<int> getBirthRules() const;

    void setUniverseName(const std::string &name);

    void setBoardWidth(const int &width);

    void setBoardHeight(const int &height);

    void addSurvivalRule(const int &i);

    void addBirthRule(const int &i);

    void addCell(const int &x, const int &y);
};


