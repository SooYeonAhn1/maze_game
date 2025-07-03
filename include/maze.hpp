#ifndef MAZE_HPP
#define MAZE_HPP

#include <cstdlib>
#include <iostream>
#include <vector>

struct Cell {
    bool visited = false;
    bool is_wall = true;
};

class Maze {
    public:
        Maze() = default;
        Maze() = default;
        Maze(int row, int col);
        Maze(const Maze& other);
        Maze& operator=(const Maze& other);
        Maze(const Maze& other);
        Maze& operator=(const Maze& other);
        void generate();
        int getRow() const { return row; };
        int getCol() const { return col; };
        const std::vector<std::vector<Cell>>& getMaze() const { return maze; };    
        bool is_walkable(int r, int c) const;
        bool is_finished() const;
        void move_player(int dr, int dc);
        int getPlayerRow() const { return p_row; }
        int getPlayerCol() const { return p_col; }
    private:
        bool row_bound(int row) const;
        bool col_bound(int row) const;
        bool row_bound(int row) const;
        bool col_bound(int row) const;
        std::vector<std::vector<Cell>> maze;
        std::vector<std::vector<int>> path;
        int row;
        int col;
        int p_row;
        int p_col;
        int p_row;
        int p_col;
        friend std::ostream& operator<<(std::ostream& os, const Maze& maze);
};


#endif