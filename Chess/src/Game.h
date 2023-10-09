#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <iostream>
#include <vector>

#include "Player.h"
#include "Board.h"
#include "Pieces/Pawn.h"

namespace Chess {
    class Game {
    public:
        Game();
        void play(std::istream& in = std::cin, std::ostream& out = std::cout);

    private:
        int curr_player;
        int turn;
        std::vector<Player> players;
        Board board;

        // PRIVATE FUNCTIONS
        bool is_game_over() const;
        bool is_draw() const;
        bool is_stalemate() const;
        bool insufficient_material() const;
        void update_all_pieces_possible_moves();
        void update_all_pieces_checker_status();
        void update_pawn_just_moved_status();
        void curr_player_takes_turn(std::istream& in, std::ostream& out);
        void switch_player();
        void declare_results(std::ostream& out);
        void update_state_of_every_piece();
    };
}

#endif