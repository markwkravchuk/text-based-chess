#include "Knight.h"
#include "Board.h"
#include "Player.h"

// CONSTRUCTORS

Chess::Knight::Knight(Player& owner, Square& square_occupying, Board& board)
             : Piece(PieceType::KNIGHT, owner, square_occupying, board) {
    symbol = (piece_color == Color::WHITE) ? 'N' : 'n';
}

// PUBLIC METHODS

void Chess::Knight::create_possible_moves() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    bool is_in_check = owner.get_opponent()->get_num_checkers() > 0;
    Direction pinned_from = get_pin_direction();
    std::vector<std::pair<char, int>> moves =
            {{1,2}, {1, -2}, {-1, 2}, {-1, -2},
             {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

    if (not is_pinned(pinned_from)) {
        for (const auto& move : moves) {
            std::pair<char, int> new_move = {static_cast<char>(file + move.first), rank + move.second};
            if (not board.at(new_move).is_null_square) {
                if (board.at(new_move).is_occupied_by_enemy(owner) or
                    not board.at(new_move).is_occupied()) {
                    if (not is_in_check) {
                        add_square_to_members(&board.at(new_move), SquareAction::ADD_TO_POSSIBLE_SQUARES);
                    } else {
                        std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
                        auto can_block_checker = std::find(squares_to_block.begin(),
                                                           squares_to_block.end(),
                                                           &board.at(new_move));
                        if (can_block_checker != squares_to_block.end()) {
                            add_square_to_members(&board.at(new_move), SquareAction::ADD_TO_POSSIBLE_SQUARES);
                        }
                    }
                }
            }
        }
    }
}

void Chess::Knight::update_squares_attacking() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    std::vector<std::pair<char, int>> moves =
            {{1,2}, {1, -2}, {-1, 2}, {-1, -2},
             {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for (auto& move : moves) {
        std::pair<char, int> new_move = {static_cast<char>(file + move.first), rank + move.second};
        if (not board.at(new_move).is_null_square) {
            add_square_to_members(&board.at(new_move), SquareAction::ADD_TO_ATTACKING);
        }
    }
}

void Chess::Knight::update_attacking_line() {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    std::vector<std::pair<char, int>> moves =
            {{1,2}, {1, -2}, {-1, 2}, {-1, -2},
             {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for (const auto& move : moves) {
        std::pair<char, int> new_move = {static_cast<char>(file + move.first), rank + move.second};
        if (board.at(new_move).is_enemy_king(owner)) {
            attacking_line.push_back(&board.at(new_move));
            return;
        }
    }
}

