#include <iostream>
#include <utility>
#include "Game.h"

// CONSTRUCTORS

Chess::Game::Game() : curr_player(0), turn(0), players(), board() {
    players.push_back(Chess::Player::create_player(1));
    players.push_back(Chess::Player::create_player(2));
    players[0].set_opponent(&players[1]);
    players[1].set_opponent(&players[0]);
    board.create_all_pieces(players);
}

//Chess::Game::Game(std::vector<Player> the_players, Chess::Board board) : curr_player(0), turn(0), players(the_players), board(board) {}

// LAYER 1

void Chess::Game::play(std::istream& in, std::ostream& out) {
    // Generate all possible moves at beginning. Do not need to worry about if checker yet
    update_all_pieces_possible_moves();
    do {
        curr_player_takes_turn(in, out);
        turn++;
        update_state_of_every_piece();
        switch_player();
    } while (not is_game_over());
    declare_results(out);
}

// LAYER 2

void Chess::Game::update_all_pieces_possible_moves() {
    for (auto& player : players) {
        player.clear_previous_move_data();
        for (auto& piece : player.get_pieces()) {
            piece->clear_previous_move_data();
            piece->update_squares_attacking();
        }
    }
    for (auto& player : players) {
        for (auto& piece : player.get_pieces()) {
            piece->create_possible_moves();
        }
    }
}

void Chess::Game::update_state_of_every_piece() {
    update_all_pieces_checker_status();
    update_pawn_just_moved_status();
    update_all_pieces_possible_moves();
}

void Chess::Game::curr_player_takes_turn(std::istream& in, std::ostream& out) {
    board.display(out);
    Move move;
    do {
        move = players[curr_player].get_move(in, out, board);
    } while (move.is_invalid());
    move.execute(in, out, board, turn);
}

void Chess::Game::switch_player() {
    curr_player = (curr_player + 1) % players.size();
}

bool Chess::Game::is_game_over() const {
    return players[curr_player].is_checkmated() or is_draw();
}

void Chess::Game::declare_results(std::ostream& out) {
    board.display(out);
    if (not is_draw()) {
        out << players[curr_player].get_opponent()->get_name() << " won the game." << std::endl; // I think this should be the other player.. CHECK FOR LATER!
    } else {
        out << "Game ended in a draw." << std::endl;
    }
}

// LAYER 3

void Chess::Game::update_all_pieces_checker_status() {
    for (auto& player : players) {
        player.clear_checkers();
        for (auto& piece : player.get_pieces()) {
            piece->set_checker_status(false);
            bool piece_is_checker = piece->check_if_checker();
            if (piece_is_checker) {
                piece->set_checker_status(true);
                player.increment_num_checkers();
                player.add_checker(piece);
            }
        }
    }
}

void Chess::Game::update_pawn_just_moved_status() {
    for (auto& player : players) {
        for (auto& piece : player.get_pieces()) {
            if (piece->get_piece_type() == PieceType::PAWN) {
                if (piece->get_just_moved_up_two()) {
                    if (piece->get_turn_moved_up_two() == turn - 1) {
                        //do nothing
                        // example: if the move on which the pawn moved up 2 was turn 5, on turn 6 I do not want to change this.
                    } else {
                        piece->set_turn_moved_up_two(-1);
                        piece->set_just_moved_up_two(false);
                    }
                }
            }
        }
    }
}

bool Chess::Game::is_draw() const {
    return is_stalemate() or insufficient_material();
}

bool Chess::Game::is_stalemate() const {
    // Returns true if current player is not in check but does not have any legal moves.
    return not players[curr_player].is_in_check() and players[curr_player].has_no_moves();
}

bool Chess::Game::insufficient_material() const {
    bool insufficient = true;
    for (const auto& player : players) {
        if (not player.cant_win()) { // If some player can still win given pieces, cannot conclude insufficient material
            insufficient = false;
        }
    }
    return insufficient;
}
