#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <vector>
#include "Board.h"
#include "Enums.h"

namespace Chess {

    class Square;
    class Player;

    class Move {
    public:
        Move();
        Move(MoveType move_type, Player* maker);
        Move(MoveType move_type, Player* maker, Square& from, Square& to);

        void execute(std::istream& in, std::ostream& out, Board& board, const int& turn);

        bool is_invalid() const;
        static bool represents_long_castle(Player& move_maker, Square& from_input, Square& to_input);
        static bool represents_short_castle(Player& move_maker, Square& from_input, Square& to_input);
        static bool represents_promotion(Player& move_maker, Square& from_input, Square& to_input);
        static bool represents_en_passant_left(Player& move_maker, Square& from_input, Square& to_input, Board& board);
        static bool represents_en_passant_right(Player& move_maker, Square& from_input, Square& to_input, Board& board);
        static bool represents_invalid(Player& move_maker, std::pair<char, int> from, std::pair<char, int> to, Board& board);

        static Move create_long_castle_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_short_castle_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_promotion_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_en_passant_left_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_en_passant_right_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_standard_move(Player& move_maker, Square& from_input, Square& to_input);
        static Move create_invalid_move(Player& move_maker);

    private:
        MoveType move_type;
        Player* maker;
        Square* from;
        Square* to;

        void update_pawn_just_moved_up_two_status(const int& turn);
        void perform_long_castle(Board& board);
        void perform_short_castle(Board& board);
        void perform_promotion(std::istream& in, std::ostream& out, Board& board);
        //void perform_en_passant_left(Board& board);
        //void perform_en_passant_right(Board& board);
        void perform_en_passant(Board& board);
        void perform_standard(const int& turn);
        static PieceType get_piece_type_wanted(std::istream& in, std::ostream& out);


        static bool is_in_bounds(std::pair<char, int> from, std::pair<char, int> to);
        static bool is_moving_own_piece(Player& move_maker, Square& from);
        static bool is_possible_move_given_piece(Square& from, Square& to);
    };
}

#endif