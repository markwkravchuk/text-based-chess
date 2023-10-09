#include "Pawn.h"
#include "Board.h"
#include "Player.h"
#include "Enums.h"

// CONSTRUCTORS

Chess::Pawn::Pawn(Player& owner, Square& square_occupying, Board& board)
           : Piece(PieceType::PAWN, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE) ? 'P' : 'p';
}

//

void Chess::Pawn::create_possible_moves() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    bool is_in_check = owner.get_opponent()->get_num_checkers() > 0;
    Direction pinned_from = get_pin_direction();

    create_standard_moves(file, rank, is_in_check, pinned_from);
    create_attacking_moves(file, rank, is_in_check, pinned_from);
    create_en_passant_moves(file, rank, is_in_check, pinned_from);
}

void Chess::Pawn::update_squares_attacking() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int direction_sign;
    switch (piece_color) {
        case Color::WHITE:
            direction_sign = 1; // positive
            break;
        case Color::BLACK:
            direction_sign = -1;
            break;
    }
    if (not board.at({file - 1, rank + direction_sign}).is_null_square) {
        add_square_to_members(&board.at({file - 1, rank + direction_sign}), SquareAction::ADD_TO_ATTACKING);
    }
    if (not board.at({file + 1, rank + direction_sign}).is_null_square) {
        add_square_to_members(&board.at({file + 1, rank + direction_sign}), SquareAction::ADD_TO_ATTACKING);
    }
}

void Chess::Pawn::create_standard_moves(char file, int rank, bool is_in_check, Direction pin_direction) {
    int direction_sign;
    switch (piece_color) {
        case Color::WHITE:
            direction_sign = 1; // positive
            break;
        case Color::BLACK:
            direction_sign = -1;
            break;
    }

    if (is_pinned_non_vertically(pin_direction)) {
        // Cannot make any moves. Pinned in a way which makes any move illegal.
    } else {
        create_move_one_up(file, rank, is_in_check, direction_sign);
        if (not has_moved) {
            create_move_two_up(file, rank, is_in_check, direction_sign);
        }
    }
}

void Chess::Pawn::create_move_one_up(char file, int rank, bool is_in_check, int direction_sign) {
    if (not board.at({file, rank + direction_sign}).is_occupied()) {
        if (not is_in_check) {
            add_square_to_members(&board.at({file, rank + direction_sign}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                                                 squares_to_block.end(),
                                                                &board.at({file, rank + direction_sign}));
            if (can_block_checker != squares_to_block.end()) {
                add_square_to_members(&board.at({file, rank + direction_sign}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            } else {
                // do nothing. That move is not within the vector of squares attacking the king
            }
        }
    }
}

void Chess::Pawn::create_move_two_up(char file, int rank, bool is_in_check, int direction_sign) {
    if (not board.at({file, rank + direction_sign * 1}).is_occupied() and
        not board.at({file, rank + direction_sign * 2}).is_occupied()) { // Move up 2 as long as both squares are empty and pawn hasn't moved yet.
        if (not is_in_check) {
            add_square_to_members(&board.at({file, rank + direction_sign * 2}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                                                 squares_to_block.end(),
                                                                      &board.at({file, rank + direction_sign * 2}));
            if (can_block_checker != squares_to_block.end()) {
                add_square_to_members(&board.at({file, rank + direction_sign * 2}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            } else {
                // do nothing. That move is not within the vector of squares attacking the king
            }
        }
    }
}

void Chess::Pawn::create_attacking_moves(char file, int rank, bool is_in_check, Direction pin_direction) {
    int rank_direction; Direction left_diagonal; Direction right_diagonal;
    switch (piece_color) {
        case Color::WHITE:
            rank_direction = 1; //upward
            left_diagonal = Direction::NW; right_diagonal = Direction::NE;
            break;
        case Color::BLACK:
            rank_direction = -1; //downward
            left_diagonal = Direction::SW; right_diagonal = Direction::SE;
            break;
    }
    if (pin_direction == Direction::NONE) {
        create_pawn_diagonal_attack_move(file, rank, rank_direction, is_in_check, Direction::W);
        create_pawn_diagonal_attack_move(file, rank, rank_direction, is_in_check, Direction::E);
    } else if (pin_direction == left_diagonal) {
        create_pawn_diagonal_attack_move(file, rank, rank_direction, is_in_check, Direction::W);
    } else if (pin_direction == right_diagonal) {
        create_pawn_diagonal_attack_move(file, rank, rank_direction, is_in_check, Direction::E);
    } else {
        // No move can be created. Pinned from a direction that would make any diagonal attack illegal.
    }
}

void Chess::Pawn::create_pawn_diagonal_attack_move(char file, int rank, int rank_direction, bool is_in_check, Direction side) {
    int file_direction;
    switch (side) {
        case Direction::W:
            file_direction = -1;
            break;
        case Direction::E:
            file_direction = 1;
            break;
        default:
            return;
    }
    if (board.at({static_cast<char>(file + file_direction), rank + rank_direction}).is_occupied_by_enemy(owner)) { // NW/SW attack
        if (not is_in_check) {
            add_square_to_members(&board.at({static_cast<char>(file + file_direction), rank + rank_direction}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                                                 squares_to_block.end(),
                                                                &board.at({static_cast<char>(file + file_direction), rank + rank_direction}));
            if (can_block_checker != squares_to_block.end()) { // CAN BLOCK CHECKER WITH THIS ATTACKING MOVE
                add_square_to_members(&board.at({static_cast<char>(file + file_direction), rank + rank_direction}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            }
        }
    }
}

void Chess::Pawn::create_en_passant_moves(char file, int rank, bool is_in_check, Direction pin_direction) {
    int rank_direction;
    Direction left_diagonal; Direction right_diagonal;
    switch (piece_color) {
        case Color::WHITE:
            rank_direction = 1; // upward
            left_diagonal = Direction::NW; right_diagonal = Direction::NE;
            break;
        case Color::BLACK:
            rank_direction = -1; // downward
            left_diagonal = Direction::SW; right_diagonal = Direction::SE;
            break;
    }

    if (pin_direction == Direction::NONE) {
        create_pawn_en_passant_move(file, rank, rank_direction, is_in_check, Direction::W);
        create_pawn_en_passant_move(file, rank, rank_direction, is_in_check, Direction::E);
    } else if (pin_direction == left_diagonal) {
        create_pawn_en_passant_move(file, rank, rank_direction, is_in_check, Direction::W);
    } else if (pin_direction == right_diagonal) {
        create_pawn_en_passant_move(file, rank, rank_direction, is_in_check, Direction::E);
    } else {
        // No move can be made. Pinned in a way where any en passant is illegal
    }
}

void Chess::Pawn::create_pawn_en_passant_move(char file, int rank, int rank_direction, bool is_in_check, Direction side) {
    int file_direction;
    switch (side) {
        case Direction::W:
            file_direction = -1;
            break;
        case Direction::E:
            file_direction = 1;
            break;
        default:
            return;
    }
    if (board.at({static_cast<char>(file + file_direction),rank}).is_occupied_by_enemy(owner) and
        board.at({static_cast<char>(file + file_direction),rank}).get_contents()->get_piece_type() == PieceType::PAWN and
        board.at({static_cast<char>(file + file_direction),rank}).get_contents()->get_just_moved_up_two()) {
        if (not is_in_check) {
            add_square_to_members(&board.at({static_cast<char>(file + file_direction), rank + rank_direction}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                                                 squares_to_block.end(),
                                                                &board.at({static_cast<char>(file + file_direction), rank + rank_direction}));
            if (can_block_checker != squares_to_block.end()) {
                add_square_to_members(&board.at({static_cast<char>(file + file_direction), rank + rank_direction}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            }
        }
    }
}

int Chess::Pawn::get_turn_moved_up_two() {
    return turn_moved_up_two;
}

void Chess::Pawn::update_attacking_line() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;

    attacking_line.clear();

    int rank_direction;
    switch (piece_color) {
        case Color::WHITE:
            rank_direction = 1; //upward
            break;
        case Color::BLACK:
            rank_direction = -1; //downward
            break;
    }
    if (board.at({static_cast<char>(file + 1), rank + rank_direction}).is_enemy_king(owner)) { // NE attack
        attacking_line.push_back(&board.at({static_cast<char>(file + 1), rank + rank_direction}));
    }
    if (board.at({static_cast<char>(file - 1), rank + rank_direction}).is_enemy_king(owner)) { // NW attack
        attacking_line.push_back(&board.at({static_cast<char>(file - 1), rank + rank_direction}));
    }
}
