#include "Bishop.h"
#include "Player.h"


// CONSTRUCTORS

Chess::Bishop::Bishop(Player& owner, Square& square_occupying, Board& board)
             : Piece(PieceType::BISHOP, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE ? 'B' : 'b');
}

// PUBLIC METHODS

void Chess::Bishop::create_possible_moves() {
    std::vector<Direction> directions = {Direction::NW, Direction::SW, Direction::SE, Direction::NE};
    Direction pinned_from = get_pin_direction();
    if (is_pinned(pinned_from)) {
        if (is_pinned_horizontally(pinned_from) or is_pinned_vertically(pinned_from)) { // If pinned horizontally or vertically, no moves can be made
            return;
        } else { // Pinned from some diagonal. Only allow moves towards that diagonal if they exist.
            create_directional_move(pinned_from);
        }
    } else {
        create_all_diagonal_moves();
    }
}

void Chess::Bishop::update_squares_attacking() {
    update_diagonal_squares_attacking();
}

void Chess::Bishop::update_attacking_line() {
    attacking_line.clear();
    std::vector<Direction> directions = {Direction::NW, Direction::NE, Direction::SE, Direction::SW};
    if (on_same_diagonal(*this, owner.get_opponent()->king())) {
        for (const Direction& direction : directions) {
            if (is_attacking_enemy_king_long_distance(direction)) {
                create_directional_attacking_line(direction);
            }
        }
    }
    // otherwise, there isn't any chance of attack on king. no attacking line exists
}
