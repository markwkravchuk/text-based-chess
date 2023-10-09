#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include <memory>

namespace Chess {

    class Piece;
    class Player;

    class Square {
    public:
        Square();
        Square(std::pair<char, int> location);

        std::shared_ptr<Piece> get_contents() const;
        std::pair<char, int> get_location() const;

        void set_contents(const std::shared_ptr<Piece>& new_contents);

        // STATUS BOOLEANS
        bool operator== (const Square& rhs);
        bool operator< (const Square& rhs);
        bool is_occupied();
        bool is_occupied_by_enemy(Player& maker);
        bool is_occupied_by_self(Player& maker);
        bool is_under_attack_by_enemy(Player& maker);
        bool is_valid_square_for_king(Player& owner);
        bool is_enemy_king(Player& maker);
        bool is_null_square;

    private:
        std::shared_ptr<Piece> contents;
        std::pair<char, int> location;
    };
}

#endif