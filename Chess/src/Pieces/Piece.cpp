#include "Piece.h"
#include "Player.h"
#include "Board.h"

// CONSTRUCTORS


Chess::Piece::Piece(PieceType piece_type, Player& owner, std::reference_wrapper<Square> square_occupying, Board& board) :
        owner(owner), piece_type(piece_type), symbol('-'), board(board), is_checker(false), has_moved(false), just_moved(false), turn_moved_up_two(-1), square_occupying(square_occupying), attacking_line(), possible_squares(), squares_attacking() {
    piece_color = owner.get_player_color();
}

// PUBLIC METHODS

void Chess::Piece::clear_previous_move_data() {
    if (not is_checker) {
        attacking_line.clear();
    }
    possible_squares.clear();
    squares_attacking.clear();
}

void Chess::Piece::set_board(Board& the_board) {
    board = the_board;
}

bool Chess::Piece::is_pinned(Direction direction) {
    return direction != Direction::NONE;
}

bool Chess::Piece::is_pinned_vertically(Direction direction) {
    return direction == Direction::N or
           direction == Direction::S;
}

bool Chess::Piece::is_pinned_horizontally(Direction direction) {
    return direction == Direction::E or
           direction ==Direction::W;
}
/*
bool Chess::Piece::is_pinned_diagonally() const {
    return is_pinned_from(Direction::NW) or
            is_pinned_from(Direction::SW) or
            is_pinned_from(Direction::SE) or
            is_pinned_from(Direction::NE);
}
*/
bool Chess::Piece::is_pinned_diagonally(Direction direction) {
    return direction == Direction::NW or
           direction == Direction::SW or
           direction == Direction::SE or
           direction == Direction::NE;
}

bool Chess::Piece::is_pinned_non_vertically(Direction direction) {
    return direction == Direction::NE or
           direction == Direction::E or
           direction == Direction::SE or
           direction == Direction::SW or
           direction == Direction::W or
           direction == Direction::NW;
}

bool Chess::Piece::check_if_checker() {
    // Find if this piece has a possible move at the location of the enemy king
    update_attacking_line();
    return not attacking_line.empty();
}

// GETTERS

Chess::PieceType Chess::Piece::get_piece_type() const {
    return piece_type;
}

Chess::Color Chess::Piece::get_piece_color() const {
    return piece_color;
}

char Chess::Piece::get_symbol() const {
    return symbol;
}

Chess::Square& Chess::Piece::get_occupied_square() const {
    return square_occupying;
}

bool Chess::Piece::get_has_moved() const {
    return has_moved;
}

bool Chess::Piece::get_just_moved_up_two() const {
    return just_moved;
}

std::vector<Chess::Square*> Chess::Piece::get_attacking_line() const {
    return attacking_line;
}

std::vector<Chess::Square*> Chess::Piece::get_possible_squares() const {
    return possible_squares;
}

std::vector<Chess::Square*> Chess::Piece::get_squares_attacking() const {
    return squares_attacking;
}

// SETTERS

void Chess::Piece::set_occupied_square(Square& to) {
    square_occupying = std::ref(to);
}

void Chess::Piece::set_has_moved() {
    has_moved = true;
}

void Chess::Piece::set_just_moved_up_two(bool value) {
    just_moved = value;
}

// PROTECTED METHODS

void Chess::Piece::create_directional_move(const Direction& direction) {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    bool is_in_check = owner.get_opponent()->get_num_checkers() > 0;
    int file_sign = -999; int rank_sign = -999; int i = 1;

    assign_variables_to_direction(direction, file_sign, rank_sign);

    while (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square and
           not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied()) {
        if (not is_in_check) {
            add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                                                 squares_to_block.end(),
                                                                &board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}));
            if (can_block_checker != squares_to_block.end()) { // IF CAN BLOCK THIS MOVE
                add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            }
        }
        i++;
    }
    // at this point, we are out of bounds or the square is occupied. if by enemy, allow for that move but otherwise its allied piece
    if (board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied_by_enemy(owner)) {
        if (not is_in_check) {
            add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
        } else {
            std::vector<Square*> squares_to_block = owner.get_opponent()->get_checkers()[0]->get_attacking_line();
            auto can_block_checker = std::find(squares_to_block.begin(),
                                               squares_to_block.end(),
                                               &board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}));
            if (can_block_checker != squares_to_block.end()) { // IF CAN BLOCK THIS MOVE
                add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_POSSIBLE_SQUARES);
            }
        }
    }
}

void Chess::Piece::create_directional_attacking_line(const Direction& direction) {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int file_sign = -999; int rank_sign = -999; int i = 1;

    assign_variables_to_direction(direction, file_sign, rank_sign);


    while (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied()) {
        attacking_line.push_back(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}));
        i++;
    }
    attacking_line.push_back(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i})); //includes that king itself.
}

void Chess::Piece::create_all_diagonal_moves() {
    std::vector<Direction> directions = {Direction::NW, Direction::SW, Direction::SE, Direction::NE};
    for (const auto& direction : directions) {
        create_directional_move(direction);
    }
}

void Chess::Piece::create_all_straight_moves() {
    std::vector<Direction> directions = {Direction::N, Direction::E, Direction::S, Direction::W};
    for (const auto& direction : directions) {
        create_directional_move(direction);
    }
}

void Chess::Piece::update_straight_squares_attacking() {
    std::vector<Direction> directions = {Direction::N, Direction::E, Direction::S, Direction::W};
    for (const auto& direction : directions) {
        update_directional_squares_attacking(direction);
    }
}

void Chess::Piece::update_diagonal_squares_attacking() {
    std::vector<Direction> directions = {Direction::NW, Direction::SW, Direction::SE, Direction::NE};
    for (const auto& direction : directions) {
        update_directional_squares_attacking(direction);
    }
}

void Chess::Piece::update_directional_squares_attacking(const Direction& direction) {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int file_sign = -999; int rank_sign = -999; int i = 1;

    assign_variables_to_direction(direction, file_sign, rank_sign);

    while (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square and
           not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied()) {
        add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_ATTACKING);
        ++i;
    }

    // Either out of bounds or this square is occupied. If not out of bounds, add this square.
    if (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square) {
        add_square_to_members(&board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}), SquareAction::ADD_TO_ATTACKING);
    }
}

bool Chess::Piece::is_pinned_from(const Direction& direction) const {
    Direction opposite_direction = get_opposite_direction(direction);
    return is_attacked_from_long_distance(direction) and
           are_no_pieces_between_self_and_king(opposite_direction);
}

bool Chess::Piece::is_attacked_from_long_distance(const Direction& direction) const {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int file_sign = -999; int rank_sign = -999; int i = 1; bool diagonal;

    assign_variables_to_direction(direction, file_sign, rank_sign, diagonal);

    while (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square and
           not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied()) {
        i++;
    }
    if (board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square) {
        return false;
    } else {
        if (diagonal) { // Diagonal threat
            return is_bishop_or_queen(board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}));
        } else {       // Horizontal/vertical threat
            return is_rook_or_queen(board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}));
        }
    }
}

bool Chess::Piece::is_attacking_enemy_king_long_distance(const Direction& direction) {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int file_sign = -999; int rank_sign = -999; int i = 1; bool diagonal;

    assign_variables_to_direction(direction, file_sign, rank_sign, diagonal);

    while (not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_occupied() and
           not board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square) {
        i++;
    }
    if (board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).is_null_square) {
        return false;
    } else {
        return board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).get_contents()->get_piece_type() == PieceType::KING
               and this->get_piece_color() != board.at({static_cast<char>(file + file_sign * i), rank + rank_sign * i}).get_contents()->get_piece_color();
    }
}

bool Chess::Piece::are_no_pieces_between_self_and_king(const Direction& direction) const {
    char file = square_occupying.get().get_location().first;
    int rank = square_occupying.get().get_location().second;
    int file_sign = -999; int rank_sign = -999; int i = 1;

    assign_variables_to_direction(direction, file_sign, rank_sign);

    while (not board.at({file + file_sign * i, rank + rank_sign * i}).is_occupied() and
           not board.at({file + file_sign * i, rank + rank_sign * i}).is_null_square) {
        ++i;
    }
    if (board.at({file + file_sign * i, rank + rank_sign * i}).is_null_square) {
        return false;
    } else {
        return board.at({file + file_sign * i, rank + rank_sign * i}).get_contents()->get_piece_type() == PieceType::KING and
               board.at({file + file_sign * i, rank + rank_sign * i}).get_contents()->get_piece_color() == owner.get_player_color();
    }
}

Chess::Direction Chess::Piece::get_opposite_direction(const Direction& direction) {
    if (direction == Direction::N) {
        return Direction::S;
    } else if (direction == Direction::NW) {
        return Direction::SE;
    } else if (direction == Direction::W) {
        return Direction::E;
    } else if (direction == Direction::SW) {
        return Direction::NE;
    } else if (direction == Direction::S) {
        return Direction::N;
    } else if (direction == Direction::SE) {
        return Direction::NW;
    } else if (direction == Direction::E) {
        return Direction::W;
    } else if (direction == Direction::NE) {
        return Direction::SW;
    } else {
        return Direction::NONE;
    }
}

void Chess::Piece::assign_variables_to_direction(const Direction& direction, int& file_sign, int& rank_sign) {
    if (direction == Direction::N) {
        file_sign = 0;
        rank_sign = 1;
    } else if (direction == Direction::NE) {
        file_sign = 1;
        rank_sign = 1;
    } else if (direction == Direction::E) {
        file_sign = 1;
        rank_sign = 0;
    } else if (direction == Direction::SE) {
        file_sign = 1;
        rank_sign = -1;
    } else if (direction == Direction::S) {
        file_sign = 0;
        rank_sign = -1;
    } else if (direction == Direction::SW) {
        file_sign = -1;
        rank_sign = -1;
    } else if (direction == Direction::W) {
        file_sign = -1;
        rank_sign = 0;
    } else if (direction == Direction::NW) {
        file_sign = -1;
        rank_sign = 1;
    }
}

void Chess::Piece::assign_variables_to_direction(const Direction& direction, int& file_sign, int& rank_sign, bool& diagonal) {
    if (direction == Direction::N) {
        file_sign = 0;
        rank_sign = 1;
        diagonal = false;
    } else if (direction == Direction::NE) {
        file_sign = 1;
        rank_sign = 1;
        diagonal = true;
    } else if (direction == Direction::E) {
        file_sign = 1;
        rank_sign = 0;
        diagonal = false;
    } else if (direction == Direction::SE) {
        file_sign = 1;
        rank_sign = -1;
        diagonal = true;
    } else if (direction == Direction::S) {
        file_sign = 0;
        rank_sign = -1;
        diagonal = false;
    } else if (direction == Direction::SW) {
        file_sign = -1;
        rank_sign = -1;
        diagonal = true;
    } else if (direction == Direction::W) {
        file_sign = -1;
        rank_sign = 0;
        diagonal = false;
    } else if (direction == Direction::NW) {
        file_sign = -1;
        rank_sign = 1;
        diagonal = true;
    }
}

bool Chess::Piece::is_bishop_or_queen(Square& square) const {
    return (square.get_contents()->get_piece_type() == PieceType::BISHOP and square.get_contents()->get_piece_color() != owner.get_player_color())
        or (square.get_contents()->get_piece_type() == PieceType::QUEEN and square.get_contents()->get_piece_color() != owner.get_player_color());
}

bool Chess::Piece::is_rook_or_queen(Square& square) const {
    return (square.get_contents()->get_piece_type() == PieceType::ROOK and square.get_contents()->get_piece_color() != owner.get_player_color())
        or (square.get_contents()->get_piece_type() == PieceType::QUEEN and square.get_contents()->get_piece_color() != owner.get_player_color());
}

bool Chess::Piece::on_same_diagonal(Chess::Piece& first_piece, const std::shared_ptr<Piece>& second_piece) {
    return abs(first_piece.get_occupied_square().get_location().first - second_piece->get_occupied_square().get_location().first) ==
           abs(first_piece.get_occupied_square().get_location().second - second_piece->get_occupied_square().get_location().second);
}

bool Chess::Piece::on_same_file(Chess::Piece& first_piece, const std::shared_ptr<Piece>& second_piece) {
    return first_piece.get_occupied_square().get_location().first == second_piece->get_occupied_square().get_location().first;
}

bool Chess::Piece::on_same_rank(Chess::Piece& first_piece, const std::shared_ptr<Piece>& second_piece) {
    return first_piece.get_occupied_square().get_location().second == second_piece->get_occupied_square().get_location().second;
}

void Chess::Piece::update_attacking_line() {
    attacking_line.clear();
}

int Chess::Piece::get_turn_moved_up_two() {
    return -1;
}

void Chess::Piece::set_turn_moved_up_two(int negative_forever) {
    turn_moved_up_two = negative_forever;
}

bool Chess::Piece::operator== (const Piece& rhs) {
    return square_occupying.get() == rhs.get_occupied_square();
}

void Chess::Piece::set_checker_status(bool status) {
    is_checker = status;
}

Chess::Direction Chess::Piece::get_pin_direction() {
    std::vector<Direction> directions = {Direction::N, Direction::NE, Direction::E, Direction::SE,
                                         Direction::S, Direction::SW, Direction::W, Direction::NW};
    for (auto& direction : directions) {
        if (is_pinned_from(direction)) {
            return direction;
        }
    }
    return Direction::NONE;
}

void Chess::Piece::add_square_to_members(Square* square, SquareAction action) {
    switch (action) {
        case SquareAction::ADD_TO_ATTACKING:
            squares_attacking.push_back(square);
            owner.get_squares_attacking().insert(square);
            break;
        case SquareAction::ADD_TO_POSSIBLE_SQUARES:
            possible_squares.push_back(square);
            owner.get_squares_can_go_to().insert(square);
            break;
        case SquareAction::ADD_TO_BOTH:
            possible_squares.push_back(square);
            owner.get_squares_can_go_to().insert(square);
            squares_attacking.push_back(square);
            owner.get_squares_attacking().insert(square);
    }
}
