#ifndef CHESS_KNIGHT_H
#define CHESS_KNIGHT_H

#include "Piece.h"

namespace Chess {
    class Knight : public Piece {
    public:
        Knight(Player& owner, Square& square_occupying, Board& board);
        void create_possible_moves() override;
        void update_squares_attacking() override;
        void update_attacking_line() override;
    };
}

#endif