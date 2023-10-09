#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "Piece.h"

namespace Chess {
    class Queen : public Piece {
    public:
        Queen(Player& owner, Square& square_occupying, Board& board);
        void create_possible_moves() override;
        void update_squares_attacking() override;
        void update_attacking_line() override;
    };

}

#endif