#pragma once

class Cell {
private:
    int x;
    int y;
public:
    Cell() = default;

    Cell(int x, int y);

    bool operator<(const Cell &other) const;
};
