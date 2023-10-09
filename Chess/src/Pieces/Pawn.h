#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

namespace Chess {
    class Pawn : public Piece {
    public:
        Pawn(Player& owner, Square& square_occupying, Board& board);

        void create_possible_moves() override;
        void update_squares_attacking() override;
        void update_attacking_line() override;

        // GETTERS
        int get_turn_moved_up_two() override;

    private:
        void create_standard_moves(char file, int rank, bool is_in_check, Direction pin_direction);
        void create_move_one_up(char file, int rank, bool is_in_check, int direction_sign);
        void create_move_two_up(char file, int rank, bool is_in_check, int direction_sign);

        void create_attacking_moves(char file, int rank, bool is_in_check, Direction pin_direction);
        void create_pawn_diagonal_attack_move(char file, int rank, int rank_direction, bool is_in_check, Direction side);

        void create_en_passant_moves(char file, int rank, bool is_in_check, Direction pin_direction);
        void create_pawn_en_passant_move(char file, int rank, int rank_direction, bool is_in_check, Direction side);
    };

}

#endif //CHESS_PAWN_H
