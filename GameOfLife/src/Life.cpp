#include "Life.h"
#include <iostream>

int Life::countLivingNeighbours(int x, int y) const {
    int count = 0;
    int gridWidth = this->boardWidth;
    int gridHeight = this->boardHeight;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int neighbourX = (x + dx + gridWidth) % gridWidth;
            int neighbourY = (y + dy + gridHeight) % gridHeight;

            if (cellExists(neighbourX, neighbourY)) {
                ++count;
            }
        }
    }
    return count;
}

bool Life::cellExists(const int &x, const int &y) const {
    Cell cell(x, y);
    return this->liveCells.count(cell) > 0;
}

void Life::advanceToNextGeneration() {
    std::set<Cell> nextGeneration;

    for (int x = 0; x < this->boardWidth; ++x) {
        for (int y = 0; y < this->boardHeight; ++y) {
            int livingNeighbours = countLivingNeighbours(x, y);

            if (cellExists(x, y)) {
                if (this->survivalRules.count(livingNeighbours) > 0) {
                    nextGeneration.insert(Cell(x, y));
                }
            }

            else if (this->birthRules.count(livingNeighbours) > 0) {
                nextGeneration.insert(Cell(x, y));
            }
        }
    }

    this->liveCells = std::move(nextGeneration);
}

void Life::printUniverse() const {
    std::cout << universeName << std::endl;

    std::cout << "Board size: " << boardWidth << " x " << boardHeight << std::endl;

    std::cout << "Birth / Survival: ";
    for (const auto& rule : birthRules) {
        std::cout << rule;
    }
    std::cout << "/";
    for (const auto& rule : survivalRules) {
        std::cout << rule;
    }
    std::cout << std::endl;

    for (int i = 0; i < boardWidth + 2; ++i) {
        std::cout << "# ";
    }
    std::cout << std::endl;
    for (int y = 0; y < boardHeight; ++y) {
        std::cout << "# ";
        for (int x = 0; x < boardWidth; ++x) {
            std::cout << (cellExists(x, y) ? "O " : "  ");
        }
        std::cout << "#" << std::endl;
    }
    for (int i = 0; i < boardWidth + 2; ++i) {
        std::cout << "# ";
    }
    std::cout << std::endl;
}

std::string Life::getUniverseName() const {
    return universeName;
}

int Life::getBoardWidth() const {
    return boardWidth;
}

int Life::getBoardHeight() const {
    return boardHeight;
}

std::set<int> Life::getSurvivalRules() const {
    return survivalRules;
}

std::set<int> Life::getBirthRules() const {
    return birthRules;
}

void Life::setUniverseName(const std::string &name) {
    this->universeName = name;
}

void Life::setBoardWidth(const int &gridWidth) {
    if (gridWidth <= 0) {
        throw std::invalid_argument("Board width must be positive.");
    }
    this->boardWidth = gridWidth;
}

void Life::setBoardHeight(const int &gridHeight) {
    if (gridHeight <= 0) {
        throw std::invalid_argument("Board height must be positive.");
    }
    this->boardHeight = gridHeight;
}

void Life::addSurvivalRule(const int &i) {
    if (i < 0 || i > 8) {
        throw std::invalid_argument("Survival rule must be between 0 and 8.");
    }
    survivalRules.insert(i);
}

void Life::addBirthRule(const int &i) {
    if (i < 0 || i > 8) {
        throw std::invalid_argument("Birth rule must be between 0 and 8.");
    }
    birthRules.insert(i);
}

void Life::addCell(const int &x, const int &y) {
    if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight) {
        throw std::out_of_range("Cell coordinates must be within the board size.");
    }
    Cell cell(x, y);
    liveCells.insert(cell);
}
