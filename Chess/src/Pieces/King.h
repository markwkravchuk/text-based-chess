#ifndef CHESS_KING_H
#define CHESS_KING_H

#include <set>
#include <algorithm>

#include "Helpers.h"
#include "Piece.h"

namespace Chess {
    class King : public Piece {
    public:
        King(Player& owner, Square& square_occupying, Board& board);
        void create_possible_moves() override;
        void update_squares_attacking() override;

    private:
        void create_general_moves(char file, int rank);
        void create_castling_moves(char file, int rank);

        void create_short_castle_move(char file, int rank);
        void create_long_castle_move(char file, int rank);
        bool are_short_castle_conditions_met(char file, int rank);
        bool are_long_castle_conditions_met(char file, int rank);
    };
}

#endif