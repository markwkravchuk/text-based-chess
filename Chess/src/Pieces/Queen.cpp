#include "Queen.h"
#include "Player.h"

// CONSTRUCTORS

Chess::Queen::Queen(Player& owner, Square& square_occupying, Board& board)
            : Piece(PieceType::QUEEN, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE) ? 'Q' : 'q';
}

// PUBLIC METHODS

void Chess::Queen::create_possible_moves() {
    std::vector<Direction> directions = {Direction::N, Direction::NE, Direction::E, Direction::SE,
                                           Direction::S, Direction::SW, Direction::W, Direction::NW};
    Direction pinned_from = get_pin_direction();

    if (is_pinned(pinned_from)) { // create moves that only move towards the pin (and thus hold it)
        create_directional_move(pinned_from);
    } else { // not pinned, create all moves.
        create_all_diagonal_moves();
        create_all_straight_moves();
    }
}

void Chess::Queen::update_squares_attacking() {
    update_straight_squares_attacking();
    update_diagonal_squares_attacking();
}

void Chess::Queen::update_attacking_line() {
    std::vector<Direction> directions;
    attacking_line.clear();
    if (on_same_rank(*this, owner.get_opponent()->king())) {
        directions = {Direction::E, Direction::W};
    } else if (on_same_file(*this, owner.get_opponent()->king())) {
        directions = {Direction::N, Direction::S};
    } else if (on_same_diagonal(*this, owner.get_opponent()->king())) {
        directions = {Direction::NE, Direction::SE, Direction::SW, Direction::NW};
    } else {
        return;
    }
    for (const Direction& direction : directions) {
        if (is_attacking_enemy_king_long_distance(direction)) {
            create_directional_attacking_line(direction);
            return;
        }
    }
}

// PRIVATE METHODS