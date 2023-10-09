#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

namespace Chess {
    class Bishop : public Piece {
    public:
        Bishop(Player& owner, Square& square_occupying, Board& board);
        void create_possible_moves() override;
        void update_squares_attacking() override;
        void update_attacking_line() override;
    };

}

#endif