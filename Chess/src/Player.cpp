#include "Player.h"

// CONSTRUCTORS

Chess::Player Chess::Player::create_player(int player_number) {
    switch (player_number) {
        case 1:
            return {"White", Color::WHITE};
        case 2:
            return {"Black", Color::BLACK};
        default:
            return {"Error", Color::WHITE};
    }
}

Chess::Player::Player(std::string name, Chess::Color color) : name(std::move(name)), color(color), pieces(), opponent(nullptr), squares_can_go_to(), squares_attacking(), checkers(), num_checkers(0) {}

// LAYER 1

void Chess::Player::set_opponent(Chess::Player* player) {
    opponent = player;
}

void Chess::Player::give_piece(const std::shared_ptr<Piece>& piece) {
    pieces.push_back(piece);
}

//

void Chess::Player::clear_previous_move_data() {
    squares_can_go_to.clear();
    squares_attacking.clear();
}

Chess::Move Chess::Player::get_move(std::istream& in, std::ostream& out, Board& board) {
    // Get the square want to move from
    out << name << ", enter the square you would like to move from: ";
    std::pair<char, int> location_from = get_location_from_input(in);

    // Get the square want to move to
    out << name << ", enter the square you would like to move to: ";
    std::pair<char, int> location_to = get_location_from_input(in);

    return create_move_from_input(*this, location_from, location_to, board);
}

// LAYER 2

std::pair<char, int> Chess::Player::get_location_from_input(std::istream& in) {
    std::string square_string;
    std::getline(in, square_string);

    // Convert input to square location on board in numbers starting at 0
    char file = square_string[0];
    int rank = square_string[1] - '0';
    return std::make_pair(file, rank);
}

Chess::Move Chess::Player::create_move_from_input(Player& maker, std::pair<char, int> from, std::pair<char, int> to, Board& board) {
    if (Move::represents_invalid(maker, from, to, board)) {
        return Move::create_invalid_move(maker);
    } else if (Move::represents_long_castle(maker, board.at(from), board.at(to))) {
        return Move::create_long_castle_move(maker, board.at(from), board.at(to));
    } else if (Move::represents_short_castle(maker, board.at(from), board.at(to))) {
        return Move::create_short_castle_move(maker, board.at(from), board.at(to));
    } else if (Move::represents_promotion(maker, board.at(from), board.at(to))) {
        return Move::create_promotion_move(maker, board.at(from), board.at(to));
    } else if (Move::represents_en_passant_left(maker, board.at(from), board.at(to), board)) {
        return Move::create_en_passant_left_move(maker, board.at(from), board.at(to));
    } else if (Move::represents_en_passant_right(maker, board.at(from), board.at(to), board)) {
        return Move::create_en_passant_right_move(maker, board.at(from), board.at(to));
    } else {
        return Move::create_standard_move(maker, board.at(from), board.at(to));
    }
}

// LAYER 3

std::shared_ptr<Chess::Piece> Chess::Player::king() const {
    return pieces[0]; // king stored at 0th index since initialization.
}

bool Chess::Player::is_checkmated() const {
    return is_in_check() and has_no_moves();
}

bool Chess::Player::is_in_check() const {
    return opponent->get_num_checkers() > 0;
}

bool Chess::Player::has_no_moves() const {
    return squares_can_go_to.empty();
}

/*
bool Chess::Player::is_overlapping(const std::vector<Square*>& vector_of_squares) const {
    if (squares_can_go_to.size() != vector_of_squares.size()) {
        return false;
    }
    for (const auto& square : vector_of_squares) {
        if (squares_can_go_to.find(square) == squares_can_go_to.end()) {
            return false;
        }
    }
    return true;
}
*/

bool Chess::Player::cant_win() const {
    if (pieces.size() == 1) { // JUST A KING LEFT
        return true;
    } else if (pieces.size() == 2){ // LOOKING FOR KING+KNIGHT or KING+BISHOP
        if (pieces[1]->get_piece_type() == PieceType::KNIGHT or pieces[1]->get_piece_type() == PieceType::BISHOP) {
            return true;
        }
    }
    return false;
}

void Chess::Player::increment_num_checkers() {
    num_checkers += 1;
}

void Chess::Player::add_checker(const std::shared_ptr<Chess::Piece>& piece) {
    checkers.push_back(piece);
}

// GETTERS

std::vector<std::shared_ptr<Chess::Piece>>& Chess::Player::get_pieces() {
    return pieces;
}

std::string Chess::Player::get_name() const {
    return name;
}

Chess::Player* Chess::Player::get_opponent() const {
    return opponent;
}

int Chess::Player::get_num_checkers() const {
    return num_checkers;
}

Chess::Color Chess::Player::get_player_color() const {
    return color;
}

std::set<Chess::Square*>& Chess::Player::get_squares_can_go_to() {
    return squares_can_go_to;
}

std::set<Chess::Square*>& Chess::Player::get_squares_attacking() {
    return squares_attacking;
}

std::vector<std::shared_ptr<Chess::Piece>> Chess::Player::get_checkers() const {
    return checkers;
}

void Chess::Player::clear_checkers() {
    checkers.clear();
    num_checkers = 0;
}
