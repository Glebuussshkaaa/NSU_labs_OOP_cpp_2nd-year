#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <limits>
#include <stdexcept>

#include "TupleUtility.h"

template<typename ...Args>
class CSVParser {
private:
    std::ifstream &CSVFileStream;
    char columnSeparator;
    char rowSeparator;
    char wrapperSymbol;
    int currentLineNumber = 0;

    std::string getRow() {
        std::string row;
        std::getline(CSVFileStream, row, rowSeparator);
        ++currentLineNumber;
        return row;
    }

    std::vector<std::string> getRowCells(const std::string &row) {
        std::vector<std::string> rowCells;
        std::string cell;
        bool isInWrapper = false;

        for (const char &sym: row) {
            if (sym == wrapperSymbol) {
                isInWrapper = !isInWrapper;
            } else if (sym == columnSeparator && !isInWrapper) {
                rowCells.push_back(cell);
                cell.clear();
            } else {
                cell += sym;
            }
        }

        if (!cell.empty()) {
            rowCells.push_back(cell);
        }

        return rowCells;
    }

    void skipLines(unsigned int NumSkipLines) {
        std::string skippedLine;
        for (unsigned int i = 0; i < NumSkipLines; ++i) {
            if (!std::getline(CSVFileStream, skippedLine)) {
                throw std::ifstream::failure("File does not contain " + std::to_string(NumSkipLines) + " lines");
            }
        }
    }

public:
    class InputIterator;

    explicit CSVParser(std::ifstream &CSVFileStream, unsigned int NumSkipLines = 0, char columnSeparator = ',',
                       char rowSeparator = '\n',
                       char wrapperSymbol = '\"') :
            CSVFileStream(CSVFileStream),
            columnSeparator(columnSeparator),
            rowSeparator(rowSeparator),
            wrapperSymbol(wrapperSymbol) {
        if (!CSVFileStream.is_open()) {
            throw std::ifstream::failure("File is not open");
        }
        skipLines(NumSkipLines);
    }

    InputIterator begin() {
        return InputIterator(*this, InputIterator::Mode::begin);
    }

    InputIterator end() {
        return InputIterator(*this, InputIterator::Mode::end);
    }
};

template<typename ...Args>
class CSVParser<Args...>::InputIterator {
public:
    enum class Mode {
        begin,
        end
    };

    InputIterator(CSVParser &CSVReader, Mode mode) : CSVReader(CSVReader) {
        switch (mode) {
            case Mode::begin:
                updatePtr();
                break;
            case Mode::end:
                tuplePtr = nullptr;
        }
    }

    std::tuple<Args...> &operator*() const {
        return *tuplePtr;
    }

    InputIterator &operator++() {
        updatePtr();
        return *this;
    }

    bool operator==(const InputIterator &b) const {
        return tuplePtr == b.tuplePtr;
    }

    bool operator!=(const InputIterator &b) {
        return tuplePtr != b.tuplePtr;
    }

private:
    CSVParser<Args...> &CSVReader;
    std::tuple<Args...> currentTuple;
    std::tuple<Args...> *tuplePtr = &currentTuple;

    void updatePtr() {
        std::string row = CSVReader.getRow();

        if (row.empty() && !CSVReader.CSVFileStream.eof()) {
            throw std::invalid_argument(std::to_string(CSVReader.currentLineNumber) + " row is empty");
        }

        if (row.empty()) {
            tuplePtr = nullptr;
            return;
        }

        std::vector<std::string> rowCells = CSVReader.getRowCells(row);

        if (rowCells.size() > sizeof...(Args)) {
            throw std::invalid_argument(
                    "Too many arguments on " + std::to_string(CSVReader.currentLineNumber) + " line");
        }
        if (rowCells.size() < sizeof...(Args)) {
            throw std::invalid_argument(
                    "Too few arguments on " + std::to_string(CSVReader.currentLineNumber) + " line");
        }

        try {
            makeTuple<Args...>(tuplePtr, rowCells);
        }
        catch (const std::invalid_argument &ex) {
            std::string errorMessage = ex.what();
            errorMessage += std::to_string(CSVReader.currentLineNumber) + " row";
            throw std::invalid_argument(errorMessage);
        }
    }
};