#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <vector>

#include "Square.h"
#include "Enums.h"

namespace Chess {

    class Board {
    public:
        Board();
        void create_all_pieces(std::vector<Player>& players);
        void display(std::ostream& out);
        Square& at(std::pair<char, int> location);

    private:
        std::vector<std::vector<Square>> board;
        Square null_square;

        void create_squares();
        void create_pieces(Player& player);
        void create_and_assign_piece(PieceType piece_type, Player& player, Square& square_occupying);

        Square& operator[](std::pair<char, int> location);
        std::vector<Square>& operator[](char file);
        std::vector<Square>& at(char file);
    };
}

#endif