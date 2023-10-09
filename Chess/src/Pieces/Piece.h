#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <iostream>
#include <vector>

#include "Square.h"
#include "Enums.h"

namespace Chess {

    class Board;

    class Piece : std::enable_shared_from_this<Piece> {
    public:
        Piece(PieceType piece_type, Player& owner, std::reference_wrapper<Square> square_occupying, Board& board);
        virtual ~Piece() = default;

        void clear_previous_move_data();
        virtual void create_possible_moves() = 0;
        virtual void update_squares_attacking() = 0;

        void set_checker_status(bool status);
        void set_turn_moved_up_two(int negative_forever);
        void set_just_moved_up_two(bool value);
        void set_occupied_square(Square& to);
        void set_has_moved();

        bool check_if_checker();
        PieceType get_piece_type() const;
        bool get_just_moved_up_two() const;
        virtual int get_turn_moved_up_two();
        char get_symbol() const;
        bool get_has_moved() const;
        Color get_piece_color() const;
        std::vector<Square*> get_possible_squares() const;
        std::vector<Square*> get_attacking_line() const;

        bool operator== (const Piece& rhs);

    protected:
        Player& owner;
        PieceType piece_type;
        Color piece_color;
        char symbol;
        Board& board;

        bool is_checker;
        bool has_moved;
        bool just_moved;
        int turn_moved_up_two;

        std::reference_wrapper<Square> square_occupying;
        std::vector<Square*> attacking_line;
        std::vector<Square*> possible_squares;
        std::vector<Square*> squares_attacking;

        Direction get_pin_direction();
        static bool is_pinned(Direction direction);
        static bool is_pinned_vertically(Direction direction);
        static bool is_pinned_horizontally(Direction direction);
        static bool is_pinned_diagonally(Direction direction);
        static bool is_pinned_non_vertically(Direction direction);

        void create_directional_move(const Direction& direction);
        void create_all_diagonal_moves();
        void create_all_straight_moves();
        void update_directional_squares_attacking(const Direction& direction);
        void update_straight_squares_attacking();
        void update_diagonal_squares_attacking();

        void create_directional_attacking_line(const Direction& direction);
        virtual void update_attacking_line();

        bool is_attacking_enemy_king_long_distance(const Direction& direction);
        static bool on_same_diagonal(Piece& first_piece, const std::shared_ptr<Piece>& second_piece);
        static bool on_same_file(Piece& first_piece, const std::shared_ptr<Piece>& second_piece);
        static bool on_same_rank(Piece& first_piece, const std::shared_ptr<Piece>& second_piece);

        void add_square_to_members(Square* square, SquareAction action);


    private:
        Square& get_occupied_square() const;
        std::vector<Square*> get_squares_attacking() const;
        static void assign_variables_to_direction(const Direction& direction, int& file_sign, int& rank_sign);
        static void assign_variables_to_direction(const Direction& direction, int& file_sign, int& rank_sign, bool& diagonal);
        static Direction get_opposite_direction(const Direction& direction);
        bool is_attacked_from_long_distance(const Direction& direction) const;
        bool are_no_pieces_between_self_and_king(const Direction& direction) const;
        bool is_pinned_from(const Direction& direction) const;

        bool is_bishop_or_queen(Square& square) const;
        bool is_rook_or_queen(Square& square) const;

        void set_board(Board& board);
    };
}

#endif