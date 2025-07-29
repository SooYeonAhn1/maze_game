#include <algorithm>
#include <unordered_set>
#include <utility>
#include <stdexcept>
#include <iostream>
#include "../include/maze.hpp"

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

Maze::Maze(int row, int col) {
    if (row % 2 != 1 || col % 2 != 1 || row < 2 || col < 2) throw std::invalid_argument("Both values have to be odd and greater than 1.");
    this->maze = std::vector<std::vector<Cell>>(row, std::vector<Cell>(col));
    this->path = std::vector<std::vector<int>>(col, std::vector<int>(2, 0));
    this->row = row;
    this->col = col;
    this->p_row = 1;
    this->p_col = 1;
    generate();
}

Maze::Maze(const Maze& other) {
    this->row = other.row;
    this->col = other.col;
    this->maze = other.maze;
    this->path = other.path;
    this->p_row = 1;
    this->p_col = 1;
    generate();
}

Maze& Maze::operator=(const Maze& other) {
    if (this != &other) {
        this->row = other.row;
        this->col = other.col;
        this->maze = other.maze;
        this->path = other.path;
        this->p_row = 1;
        this->p_col = 1;
    }
    return *this;
}

void Maze::generate() {
    auto is_valid = [&](int r, int c) {
        return row_bound(r) && col_bound(c);
    };

    std::vector<std::pair<int, int>> vfrontier;
    auto frontier = [&](int r, int c) {
        if (!is_valid(r, c)) return;
        if (maze[r][c].visited) return;
        vfrontier.emplace_back(r, c);
        maze[r][c].visited = true;
    };

    int start_r = 1, start_c = 1;
    maze[start_r][start_c].is_wall = false;
    maze[start_r][start_c].visited = true;
    frontier(start_r + 2, start_c);
    frontier(start_r - 2, start_c);
    frontier(start_r, start_c + 2);
    frontier(start_r, start_c - 2);

    while (!vfrontier.empty()) {
        std::swap(vfrontier[rand() % vfrontier.size()], vfrontier.back());
        auto [r, c] = vfrontier.back();
        vfrontier.pop_back();

        std::vector<std::pair<int, int>> neighbors;
        for (auto [dr, dc] : {std::pair{-2, 0}, {2, 0}, {0, -2}, {0, 2}}) {
            int nr = r + dr;
            int nc = c + dc;
            if (is_valid(nr, nc) && !maze[nr][nc].is_wall) {
                neighbors.emplace_back(nr, nc);
            }
        }

        if (!neighbors.empty()) {
            auto [nr, nc] = neighbors[rand() % neighbors.size()];
            int wall_r = (r + nr) / 2;
            int wall_c = (c + nc) / 2;
            maze[r][c].is_wall = false;
            maze[wall_r][wall_c].is_wall = false;

            for (auto [dr, dc] : {std::pair{-2, 0}, {2, 0}, {0, -2}, {0, 2}}) {
                frontier(r + dr, c + dc);
            }
        }
    }
}


bool Maze::row_bound(int r) const {
    return r > 0 && r < row - 1;
}

bool Maze::col_bound(int c) const {
    return c > 0 && c < col - 1;
}

std::ostream& operator<<(std::ostream& os, const Maze& m) {
    for (int i = 0; i < m.getRow(); ++i) {
        for (int j = 0; j < m.getCol(); ++j) {
            const Cell& cell = m.maze[i][j];

            if (i == m.p_row && j == m.p_col) {
                os << "@ ";  // Start point
            } else if (i == m.getRow() - 2 && j == m.getCol() - 2) {
                os << "E ";  // End point
            } else if (cell.is_wall) {
                os << "██";  // Wall
            } else {
                os << "  ";  // Path
            }
        }
        os << '\n';
    }
    return os;
}

bool Maze::is_walkable(int r, int c) const {
    return row_bound(r) && col_bound(c) && !maze[r][c].is_wall;
}

void Maze::move_player(int dr, int dc) {
    int c_row = p_row + dr;
    int c_col = p_col + dc;
    if (is_walkable(c_row, c_col)) {
        p_row = c_row;
        p_col = c_col;
    } else {
        throw std::invalid_argument("This is a wall");
    }
}

bool Maze::is_finished() const {
    return p_row == row - 2 && p_col == col - 2;
}