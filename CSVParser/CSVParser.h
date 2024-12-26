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
    std::ifstream &input_file;
    char column_delimiter;
    char row_delimiter;
    char escape_sym;
    int current_line = 0;

    std::string getRow() {
        std::string row;
        std::getline(input_file, row, row_delimiter);
        ++current_line;
        return row;
    }

    std::vector<std::string> getRowCells(const std::string &row) {
        std::vector<std::string> row_cells(1);
        std::string cell;
        bool is_escape_sym = false;

        for (const char &sym: row) {
            if (sym == escape_sym) {
                is_escape_sym = !is_escape_sym;
                continue;
            }

            if (sym == column_delimiter) {
                if (is_escape_sym) {
                    cell += sym;
                    continue;
                }
                row_cells.back() = cell;
                row_cells.emplace_back();
                cell.clear();
                continue;
            }

            cell += sym;
        }

        if (!cell.empty()) {
            row_cells.back() = cell;
        }

        return row_cells;
    }

    void skipLines(unsigned int line_offset) {
        std::string skipped_line;
        for (unsigned int i = 0; i < line_offset; ++i) {
            if (!std::getline(input_file, skipped_line)) {
                throw std::ifstream::failure("File does not contain " + std::to_string(line_offset) + " lines");
            }
        }
    }

public:
    class InputIterator;

    explicit CSVParser(std::ifstream &input_file, unsigned int line_offset = 0, char column_delimiter = ',',
                       char line_delimiter = '\n',
                       char escape_sym = '\"') :
            input_file(input_file),
            column_delimiter(column_delimiter),
            row_delimiter(line_delimiter),
            escape_sym(escape_sym) {
        if (!input_file.is_open()) {
            throw std::ifstream::failure("File is not open");
        }
        skipLines(line_offset);
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
    using value_type = std::tuple<Args...>;
    using reference = std::tuple<Args...> &;
    using pointer = std::tuple<Args...> *;

    enum class Mode {
        begin,
        end
    };

    InputIterator(CSVParser &m_parent, Mode mode) : m_parent(m_parent) {
        switch (mode) {
            case Mode::begin:
                updatePtr();
                break;
            case Mode::end:
                m_ptr = nullptr;
        }
    }

    reference operator*() const {
        return *m_ptr;
    }

    pointer operator->() {
        return m_ptr;
    }

    InputIterator &operator++() {
        updatePtr();
        return *this;
    }


    friend bool operator==(const InputIterator &a, const InputIterator &b) {
        return a.m_ptr == b.m_ptr;
    }

    friend bool operator!=(const InputIterator &a, const InputIterator &b) {
        return a.m_ptr != b.m_ptr;
    }

private:
    CSVParser<Args...> &m_parent;
    value_type tp;
    pointer m_ptr = &tp;

    void updatePtr() {
        std::string row = m_parent.getRow();
        if (row.empty() && !m_parent.input_file.eof()) {
            throw std::invalid_argument(std::to_string(m_parent.current_line) + " row is empty");
        }

        if (row.empty()) {
            m_ptr = nullptr;
            return;
        }

        std::vector<std::string> rowCells = m_parent.getRowCells(row);

        if (rowCells.size() > sizeof...(Args)) {
            throw std::invalid_argument("Too many arguments on " + std::to_string(m_parent.current_line) + " line");
        }
        if (rowCells.size() < sizeof...(Args)) {
            throw std::invalid_argument("Too few arguments on " + std::to_string(m_parent.current_line) + " line");
        }

        try {
            makeTuple<Args...>(m_ptr, rowCells);
        }
        catch (const std::invalid_argument &ex) {
            std::string msg = ex.what();
            msg += std::to_string(m_parent.current_line) + " row";
            throw std::invalid_argument(msg);
        }
    }
};