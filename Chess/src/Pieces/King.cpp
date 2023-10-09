#include "King.h"
#include "Board.h"
#include "Player.h"

// CONSTRUCTORS

Chess::King::King(Player& owner, Square& square_occupying, Board& board)
           : Piece(PieceType::KING, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE) ? 'K' : 'k';
}

// PUBLIC METHODS

void Chess::King::create_possible_moves() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    create_general_moves(file, rank);
    create_castling_moves(file, rank);
}

void Chess::King::update_squares_attacking() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    for (int file_delta = -1; file_delta <= 1; file_delta++) {
        for (int rank_delta = -1; rank_delta <= 1; rank_delta++) {
            if (not(file_delta == 0 and rank_delta == 0)) {
                if (not board.at({static_cast<char>(file + file_delta), rank + rank_delta}).is_null_square) {
                    add_square_to_members(&board.at({static_cast<char>(file + file_delta), rank + rank_delta}), SquareAction::ADD_TO_ATTACKING);
                }
            }
        }
    }
}

// PRIVATE METHODS

void Chess::King::create_general_moves(char file, int rank) {
    for (int file_delta = -1; file_delta <= 1; file_delta++) {
       for (int rank_delta = -1; rank_delta <= 1; rank_delta++) {
           if (not (file_delta == 0 and rank_delta == 0)) {
               if (board.at({static_cast<char>(file + file_delta), rank + rank_delta}).is_valid_square_for_king(owner)) {
                   add_square_to_members(&board.at({static_cast<char>(file + file_delta), rank + rank_delta}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
               }
           }
       }
   }
}

void Chess::King::create_castling_moves(char file, int rank) {
    create_short_castle_move(file, rank);
    create_long_castle_move(file, rank);
}

void Chess::King::create_short_castle_move(char file, int rank) {
    if (are_short_castle_conditions_met(file, rank)) {
        add_square_to_members(&board.at({static_cast<char>(file + 2), rank}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
    }
}

void Chess::King::create_long_castle_move(char file, int rank) {
    if (are_long_castle_conditions_met(file, rank)) {
        add_square_to_members(&board.at({static_cast<char>(file - 2), rank}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
    }
}

bool Chess::King::are_short_castle_conditions_met(char file, int rank) {
    return not has_moved and
           not owner.is_in_check() and // King hasn't moved
           not (board.at({static_cast<char>(file + 3), rank}).get_contents() == nullptr) and
           not board.at({static_cast<char>(file + 3), rank}).get_contents()->get_has_moved() and // Short rook hasn't moved
           not board.at({static_cast<char>(file + 1), rank}).is_occupied() and not board.at({static_cast<char>(file + 2), rank}).is_occupied() and // No pieces in between
           not board.at({static_cast<char>(file + 1), rank}).is_under_attack_by_enemy(owner) and not board.at({static_cast<char>(file + 2), rank}).is_under_attack_by_enemy(owner);

}

bool Chess::King::are_long_castle_conditions_met(char file, int rank) {
    return not has_moved and
           not owner.is_in_check() and
           not (board.at({static_cast<char>(file - 4), rank}).get_contents() == nullptr) and
           not board.at({static_cast<char>(file - 4), rank}).get_contents()->get_has_moved() and // Short rook hasn't moved
           not board.at({static_cast<char>(file - 3), rank}).is_occupied() and not board.at({static_cast<char>(file - 2), rank}).is_occupied() and not board.at({static_cast<char>(file - 1), rank}).is_occupied() and // No pieces in between
           not board.at({static_cast<char>(file - 2), rank}).is_under_attack_by_enemy(owner) and not board.at({static_cast<char>(file - 1), rank}).is_under_attack_by_enemy(owner);
}


