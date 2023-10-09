#include "Rook.h"
#include "Player.h"

// CONSTRUCTORS

Chess::Rook::Rook(Player& owner, Square& square_occupying, Board& board)
           : Piece(PieceType::ROOK, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE) ? 'R' : 'r';
}

// PUBLIC METHODS

void Chess::Rook::create_possible_moves() {
    std::vector<Direction> directions = {Direction::N, Direction::E, Direction::S, Direction::W};
    Direction pinned_from = get_pin_direction();

    if (is_pinned(pinned_from)) {
        if (is_pinned_diagonally(pinned_from)) {
            return; // No moves can be made.
        } else {
            create_directional_move(pinned_from);
        }
    } else { // not pinned
        create_all_straight_moves();
    }
}

void Chess::Rook::update_squares_attacking() {
    update_straight_squares_attacking();
}

void Chess::Rook::update_attacking_line() {
    std::vector<Direction> directions;
    attacking_line.clear();
    if (on_same_rank(*this, owner.get_opponent()->king())) {
        directions = {Direction::E, Direction::W};
    } else if (on_same_file(*this, owner.get_opponent()->king())) {
        directions = {Direction::N, Direction::S};
    }
    for (const Direction& direction : directions) {
        if (is_attacking_enemy_king_long_distance(direction)) {
            create_directional_attacking_line(direction);
        }
    }
}

// PRIVATE METHODS