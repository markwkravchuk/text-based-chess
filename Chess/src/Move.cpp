#include "Move.h"
#include "Square.h"
#include "Enums.h"
#include "Player.h"
#include "Pieces/Bishop.h"
#include "Pieces/Knight.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

// CONSTRUCTORS

Chess::Move::Move() : Move(MoveType::INVALID, nullptr) {}

Chess::Move::Move(Chess::MoveType move_type, Chess::Player* move_maker) : move_type(move_type), maker(move_maker), from(nullptr), to(nullptr) {}

Chess::Move::Move(Chess::MoveType move_type, Chess::Player* move_maker, Chess::Square& from, Chess::Square& to) : move_type(move_type), maker(move_maker), from(&from), to(&to) {}

// PUBLIC METHODS

void Chess::Move::execute(std::istream& in, std::ostream& out, Board& board, const int& turn) {
    if (move_type == MoveType::CASTLE_LONG) {
        perform_long_castle(board);
    } else if (move_type == MoveType::CASTLE_SHORT) {
        perform_short_castle(board);
    } else if (move_type == MoveType::PROMOTION) {
        perform_promotion(in, out, board);
    } else if (move_type == MoveType::EN_PASSANT_LEFT or move_type == MoveType::EN_PASSANT_RIGHT) {
        perform_en_passant(board);
    } else if (move_type == MoveType::STANDARD) {
        perform_standard(turn);
    } else if (move_type == MoveType::INVALID) {
        // perform_invalid(); // THIS SHOULD NEVER HAPPEN SINCE WE CHECK FOR THIS BEFORE.
    }
}

bool Chess::Move::represents_invalid(Player& move_maker, std::pair<char, int> from, std::pair<char, int> to, Board& board) {
    return not is_in_bounds(from, to) or
           not is_moving_own_piece(move_maker, board.at(from)) or
           not is_possible_move_given_piece(board.at(from), board.at(to));
}

bool Chess::Move::represents_long_castle(Player& move_maker, Square& from_input, Square& to_input) {
    switch (move_maker.get_player_color()) {
        case Color::WHITE:
            return from_input.get_location() == std::make_pair('E', 1) and
                   to_input.get_location() == std::make_pair('C', 1) and
                   from_input.get_contents()->get_piece_type() == PieceType::KING;
        case Color::BLACK:
            return from_input.get_location() == std::make_pair('E', 8) and
                   to_input.get_location() == std::make_pair('C', 8) and
                   from_input.get_contents()->get_piece_type() == PieceType::KING;
    }
}

bool Chess::Move::represents_short_castle(Player& move_maker, Square& from_input, Square& to_input) {
    switch (move_maker.get_player_color()) {
        case Color::WHITE:
            return from_input == std::make_pair('E', 1) and
                   to_input == std::make_pair('G', 1) and
                   from_input.get_contents()->get_piece_type() == PieceType::KING;
        case Color::BLACK:
            return from_input == std::make_pair('E', 8) and
                   to_input == std::make_pair('G', 8) and
                   from_input.get_contents()->get_piece_type() == PieceType::KING;
    }
}

bool Chess::Move::represents_promotion(Player& move_maker, Square& from_input, Square& to_input) {
    switch (move_maker.get_player_color()) {
        case Color::WHITE:
            return from_input.get_location().second == 7 and // 7th rank
                   to_input.get_location().second == 8 and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
        case Color::BLACK:
            return from_input.get_location().second == 2 and // 7th rank
                   to_input.get_location().second == 1 and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
    }
}

bool Chess::Move::represents_en_passant_left(Player& move_maker, Square& from_input, Square& to_input, Board& board) {
    std::pair<char, int> left_up = {from_input.get_location().first - 1, from_input.get_location().second + 1};
    std::pair<char, int> left_down = {from_input.get_location().first - 1, from_input.get_location().second - 1};
    std::pair<char, int> left = {from_input.get_location().first - 1, from_input.get_location().second};
    switch (move_maker.get_player_color()) {
        case Color::WHITE:
            return to_input.get_location() == left_up and
                   board.at(left).get_contents() != nullptr and
                   board.at(left).get_contents()->get_piece_type() == PieceType::PAWN and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
        case Color::BLACK:
            return to_input.get_location() == left_down and
                   board.at(left).get_contents() != nullptr and
                   board.at(left).get_contents()->get_piece_type() == PieceType::PAWN and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
    }
}

bool Chess::Move::represents_en_passant_right(Player& move_maker, Square& from_input, Square& to_input, Board& board) {
    std::pair<char, int> right_up = {from_input.get_location().first + 1, from_input.get_location().second + 1};
    std::pair<char, int> right_down = {from_input.get_location().first + 1, from_input.get_location().second - 1};
    std::pair<char, int> right = {from_input.get_location().first + 1, from_input.get_location().second};

    switch (move_maker.get_player_color()) {
        case Color::WHITE:
            return to_input.get_location() == right_up and
                   board.at(right).get_contents() != nullptr and
                   board.at(right).get_contents()->get_piece_type() == PieceType::PAWN and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
        case Color::BLACK:
            return to_input.get_location() == right_down and
                   board.at(right).get_contents() != nullptr and
                   board.at(right).get_contents()->get_piece_type() == PieceType::PAWN and
                   from_input.get_contents()->get_piece_type() == PieceType::PAWN;
    }
}

/*
bool Chess::Move::represents_standard(Player& move_maker, Square& from, Square& to) {
  
}
*/ // I dont think representing standard makes sense in my context?
// I know it is a valid move up to this point but not some special move.

Chess::Move Chess::Move::create_long_castle_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::CASTLE_LONG, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_short_castle_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::CASTLE_SHORT, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_promotion_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::PROMOTION, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_standard_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::STANDARD, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_en_passant_left_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::EN_PASSANT_LEFT, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_en_passant_right_move(Player& move_maker, Square& from_input, Square& to_input) {
    return {MoveType::EN_PASSANT_RIGHT, &move_maker, from_input, to_input};
}

Chess::Move Chess::Move::create_invalid_move(Player& move_maker) {
    return {MoveType::INVALID, &move_maker};
}

void Chess::Move::perform_long_castle(Board& board) {
    from->get_contents()->set_occupied_square(*to); // set the kings occupied square to the new one
    to->set_contents(from->get_contents()); // set that squares contents to king
    from->set_contents(nullptr); // set the kings old square's contents to nullptr
    switch(maker->get_player_color()) {
        case Color::WHITE: // if white, move long rook to D1
            board.at({'A', 1}).get_contents()->set_occupied_square(board.at({'D', 1}));
            board.at({'D', 1}).set_contents(board.at({'A', 1}).get_contents());
            board.at({'A', 1}).set_contents(nullptr);
            break;
        case Color::BLACK: // if black, move long rook to D8
            board.at({'A', 8}).get_contents()->set_occupied_square(board.at({'D', 8}));
            board.at({'D', 8}).set_contents(board.at({'A', 8}).get_contents());
            board.at({'A', 8}).set_contents(nullptr);
            break;
    }
}

void Chess::Move::perform_short_castle(Board& board) {
    from->get_contents()->set_occupied_square(*to);
    to->set_contents(from->get_contents());
    from->set_contents(nullptr);
    switch(maker->get_player_color()) {
        case Color::WHITE:
            board.at({'H', 1}).get_contents()->set_occupied_square(board.at({'F', 1}));
            board.at({'H', 1}).get_contents()->set_has_moved();
            board.at({'F', 1}).set_contents(board.at({'H', 1}).get_contents());
            board.at({'H', 1}).set_contents(nullptr);
            break;
        case Color::BLACK:
            board.at({'H', 8}).get_contents()->set_occupied_square(board.at({'F', 8}));
            board.at({'H', 8}).get_contents()->set_has_moved();
            board.at({'F', 8}).set_contents(board.at({'H', 8}).get_contents());
            board.at({'H', 8}).set_contents(nullptr);
            break;
    }
}

void Chess::Move::perform_promotion(std::istream& in, std::ostream& out, Board& board) {
    PieceType desired_piece_type_of_promotion = get_piece_type_wanted(in, out);
    std::shared_ptr<Piece> piece;
    switch (desired_piece_type_of_promotion) {
        case PieceType::BISHOP:
            piece = std::make_shared<Bishop>(*maker, *to, board);
            break;
        case PieceType::KNIGHT:
            piece = std::make_shared<Knight>(*maker, *to, board);
            break;
        case PieceType::QUEEN:
            piece = std::make_shared<Queen>(*maker, *to, board);
            break;
        case PieceType::ROOK:
            piece = std::make_shared<Rook>(*maker, *to, board);
            break;
        default:
            break;
    }
    if (to->is_occupied_by_enemy(*maker)) {
        auto it = std::find(maker->get_opponent()->get_pieces().begin(), maker->get_opponent()->get_pieces().end(), to->get_contents());
        if (it != maker->get_opponent()->get_pieces().end()) {
            maker->get_opponent()->get_pieces().erase(it);
        }
    }
    to->set_contents(piece);
    maker->get_pieces().push_back(piece);
    maker->get_pieces().erase(std::remove(maker->get_pieces().begin(), maker->get_pieces().end(), from->get_contents()), maker->get_pieces().end());
    from->set_contents(nullptr);
}

void Chess::Move::perform_en_passant(Board& board) {
    std::pair<char, int> side;
    switch (move_type) {
        case MoveType::EN_PASSANT_LEFT:
            side = {from->get_location().first - 1, from->get_location().second};
            break;
        case MoveType::EN_PASSANT_RIGHT:
            side = {from->get_location().first + 1, from->get_location().second};
            break;
        default:
            return;
    }
    from->get_contents()->set_occupied_square(*to); // set the attacking pawns new square
    from->get_contents()->set_has_moved();
    to->set_contents(from->get_contents()); // set that squares new piece its holding
    from->set_contents(nullptr); // empty the square it just moved from

    auto it = std::find(maker->get_opponent()->get_pieces().begin(), maker->get_opponent()->get_pieces().end(), board.at(side).get_contents());
    if (it != maker->get_opponent()->get_pieces().end()) {
        maker->get_opponent()->get_pieces().erase(it);
    }

    board.at(side).set_contents(nullptr);
}

void Chess::Move::perform_standard(const int& turn) {
    from->get_contents()->set_has_moved();
    if (to->is_occupied()) {
        auto it = std::find(maker->get_opponent()->get_pieces().begin(), maker->get_opponent()->get_pieces().end(), to->get_contents());
        if (it != maker->get_opponent()->get_pieces().end()) {
            maker->get_opponent()->get_pieces().erase(it);
        }
    }
    to->set_contents(from->get_contents());
    from->get_contents()->set_occupied_square(*to);

    if (from->get_contents()->get_piece_type() == PieceType::PAWN) {
        update_pawn_just_moved_up_two_status(turn);
    }
    from->set_contents(nullptr);
}

bool Chess::Move::is_in_bounds(std::pair<char, int> from, std::pair<char, int> to) {
    return from.first >= 'A' and from.first <= 'H' and // File is 'A' - 'H'  and
           from.second >= 1 and from.second <= 8 and  // Rank is 1 - 8
           to.first >= 'A' and to.first <= 'H' and // File is 'A' - 'H'  and
           to.second >= 1 and to.second <= 8; // Rank is 1 - 8
}

bool Chess::Move::is_moving_own_piece(Player& move_maker, Square& from_input) {
    if (from_input.get_contents() != nullptr) {
        return from_input.get_contents()->get_piece_color() == move_maker.get_player_color();
    } else {
        return false;
    }
}

bool Chess::Move::is_possible_move_given_piece(Square& from_input, Square& to_input) {
    if (from_input.get_contents() == nullptr) {
        return false;
    } else {
        for (const auto& square : from_input.get_contents()->get_possible_squares()) {
            if (square == &to_input) {
                return true;
            }
        }
        return false;
    }
}

bool Chess::Move::is_invalid() const {
    return move_type == MoveType::INVALID;
}

Chess::PieceType Chess::Move::get_piece_type_wanted(std::istream& in, std::ostream& out) {
    out << "Enter a letter for the type of piece you would like. Ex: Queen = q." << std::endl;
    std::string input_string;
    std::getline(in, input_string);
    char type_as_char = input_string[0];

    switch (type_as_char) {
        case 'b':
            return PieceType::BISHOP;
        case 'n':
            return PieceType::KNIGHT;
        case 'q':
            return PieceType::QUEEN;
        case 'r':
            return PieceType::ROOK;
        default:
            return PieceType::PAWN; // should never happen. Not promoting a pawn to a pawn ever..
    }
}

void Chess::Move::update_pawn_just_moved_up_two_status(const int& turn) {
    int delta;
    switch (to->get_contents()->get_piece_color()) {
        case Color::WHITE:
            delta = 2;
            break;
        case Color::BLACK:
            delta = -2;
    }
    if (to->get_location().second == from->get_location().second + delta) {
        to->get_contents()->set_just_moved_up_two(true);
        to->get_contents()->set_turn_moved_up_two(turn);
    }
}
