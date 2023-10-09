#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

namespace Chess {
    class Rook : public Piece {
    public:
        Rook(Player& owner, Square& square_occupying, Board& board);
        void create_possible_moves() override;
        void update_squares_attacking() override;
        void update_attacking_line() override;
    };
}

#endif