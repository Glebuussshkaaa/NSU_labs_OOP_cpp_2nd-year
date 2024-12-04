#include "Cell.h"

Cell::Cell(int x, int y) : x(x), y(y) {}

bool Cell::operator<(const Cell &other) const {
    return x < other.x || (x == other.x && y < other.y);
}
