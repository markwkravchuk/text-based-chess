#include "Square.h"

#include <utility>
#include "Pieces/Piece.h"
#include "Player.h"

// CONSTRUCTOR

Chess::Square::Square() : is_null_square(true), contents(nullptr), location({'Z', 0}) {}

Chess::Square::Square(std::pair<char, int> pair) : is_null_square(false), contents(nullptr), location(pair) {}

// PUBLIC METHODS

void Chess::Square::set_contents(const std::shared_ptr<Piece>& new_contents) {
    contents = new_contents;
}

std::shared_ptr<Chess::Piece> Chess::Square::get_contents() const {
    return contents;
}

std::pair<char, int> Chess::Square::get_location() const {
    return location;
}


bool Chess::Square::operator== (const Square& rhs) {
    return location == rhs.location;
}

bool Chess::Square::operator< (const Square& rhs) {
    if (location.first < rhs.location.first) {
        return true;
    } else if (location.first > rhs.location.first) {
        return false;
    } else {
        if (location.second < rhs.location.second) {
            return true;
        } else if (location.second > rhs.location.second) {
            return false;
        } else {
            return *this == rhs;
        }
    }
}

bool Chess::Square::is_occupied() {
    return not is_null_square and
           contents != nullptr;
}

bool Chess::Square::is_occupied_by_enemy(Player& maker) {
    return not is_null_square and
           contents != nullptr and
           maker.get_player_color() != contents->get_piece_color(); // Attackers color is opposite of piece color
}

bool Chess::Square::is_occupied_by_self(Player& maker) {
    return not is_null_square and
           contents != nullptr and
           maker.get_player_color() == contents->get_piece_color();
}

bool Chess::Square::is_enemy_king(Player& maker) {
    return not is_null_square and
           contents != nullptr and
           contents->get_piece_type() == PieceType::KING and
           maker.get_player_color() != contents->get_piece_color();
}

bool Chess::Square::is_under_attack_by_enemy(Player& maker) {
    // figure out whether the opponent can go to own square or something
    if (maker.get_opponent()->get_squares_attacking().empty()) {
        return false;
    } else {
        return not is_null_square and
               maker.get_opponent()->get_squares_attacking().find(this) !=
               maker.get_opponent()->get_squares_attacking().end();
    }
}

bool Chess::Square::is_valid_square_for_king(Player& maker) {
    return not is_null_square and
           not is_under_attack_by_enemy(maker) and
           not is_occupied_by_self(maker);
}

