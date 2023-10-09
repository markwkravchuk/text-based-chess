#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include <set>

#include "Move.h"
#include "Pieces/Piece.h"
#include "Board.h"
#include "Enums.h"

namespace Chess {

    class Player {
    public:
        static Player create_player(int player_number);
        Player(std::string name, Color color);

        Move get_move(std::istream& in, std::ostream& out, Board& board);
        void clear_previous_move_data();
        void clear_checkers();

        // GETTERS
        Player* get_opponent() const;
        std::shared_ptr<Piece> king() const;
        int get_num_checkers() const;
        std::vector<std::shared_ptr<Piece>> get_checkers() const;
        std::set<Square*>& get_squares_can_go_to();
        std::set<Square*>& get_squares_attacking();
        std::vector<std::shared_ptr<Piece>>& get_pieces();
        std::string get_name() const;
        Color get_player_color() const;

        // SETTING / GIVING
        void give_piece(const std::shared_ptr<Piece>& piece);
        void add_checker(const std::shared_ptr<Piece>& piece);
        void increment_num_checkers();
        void set_opponent(Player* player);

        // STATUS BOOLEANS
        bool is_checkmated() const;
        bool is_in_check() const;
        bool has_no_moves() const;
        bool cant_win() const;

    private:
        std::string name;
        Color color;
        std::vector<std::shared_ptr<Piece>> pieces;
        Player* opponent;

        std::set<Square*> squares_can_go_to;
        std::set<Square*> squares_attacking;
        std::vector<std::shared_ptr<Piece>> checkers;
        int num_checkers;

        static Move create_move_from_input(Player& maker, std::pair<char, int> from, std::pair<char, int> to, Board& board);
        static std::pair<char, int> get_location_from_input(std::istream& in);
    };
}

#endif