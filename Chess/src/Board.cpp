#include "Board.h"
#include "Pieces/Piece.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Knight.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"
#include "Player.h"
#include <stdexcept>

// CONSTRUCTORS

Chess::Board::Board() : null_square(Square()) {
    create_squares();
}

void Chess::Board::create_all_pieces(std::vector<Chess::Player>& players) {
    for (auto& player : players) {
        create_pieces(player);
    }
}

void Chess::Board::create_squares() {
    for (char file = 'A'; file <= 'H'; ++file) {
        std::vector<Square> squares_in_file;
        for (int rank = 1; rank <= 8; ++rank) {
            squares_in_file.push_back(Square({file, rank}));
        }
        board.push_back(squares_in_file);
    }
}

void Chess::Board::create_pieces(Player& player) {
    int pawn_rank;
    int major_piece_rank;
    switch (player.get_player_color()) {
        case Color::WHITE:
            pawn_rank = 2;
            major_piece_rank = 1;
            break;
        case Color::BLACK:
            pawn_rank = 7;
            major_piece_rank = 8;
    }
    // Give king so that it is first piece in vector "pieces" for duration of game
    create_and_assign_piece(PieceType::KING, player, this->at(std::make_pair('E', major_piece_rank)));

    // Create Pawns
    for (char file = 'A'; file <= 'H'; ++file) {
        create_and_assign_piece(PieceType::PAWN, player, this->at(std::make_pair(file, pawn_rank)));
    }
    // Create major pieces
    create_and_assign_piece(PieceType::ROOK, player, this->at(std::make_pair('A', major_piece_rank)));
    create_and_assign_piece(PieceType::ROOK, player, this->at(std::make_pair('H', major_piece_rank)));
    create_and_assign_piece(PieceType::KNIGHT, player, this->at(std::make_pair('B', major_piece_rank)));
    create_and_assign_piece(PieceType::KNIGHT, player, this->at(std::make_pair('G', major_piece_rank)));
    create_and_assign_piece(PieceType::BISHOP, player, this->at(std::make_pair('C', major_piece_rank)));
    create_and_assign_piece(PieceType::BISHOP, player, this->at(std::make_pair('F', major_piece_rank)));
    create_and_assign_piece(PieceType::QUEEN, player, this->at(std::make_pair('D', major_piece_rank)));
}

void Chess::Board::create_and_assign_piece(PieceType piece_type, Player& player, Square& square_occupying) {
    std::shared_ptr<Piece> piece;
    switch (piece_type) {
        case PieceType::KING:
            piece = std::make_shared<King>(player, square_occupying, *this);
            break;
        case PieceType::QUEEN:
            piece = std::make_shared<Queen>(player, square_occupying, *this);
            break;
        case PieceType::PAWN:
            piece = std::make_shared<Pawn>(player, square_occupying, *this);
            break;
        case PieceType::ROOK:
            piece = std::make_shared<Rook>(player, square_occupying, *this);
            break;
        case PieceType::BISHOP:
            piece = std::make_shared<Bishop>(player, square_occupying, *this);
            break;
        case PieceType::KNIGHT:
            piece = std::make_shared<Knight>(player, square_occupying, *this);
            break;
    }
    square_occupying.set_contents(piece);
    player.give_piece(piece);
}

void Chess::Board::display(std::ostream& out) {
    // Print row number followed by values of board
    // Top down printing
    out << "\n";
    for (int rank = 8; rank >= 1; --rank) {
        out << rank << "    ";
        for (char file = 'A'; file <= 'H'; ++file) {
            if (this->at(std::make_pair(file, rank)).get_contents() != nullptr) {
                out << this->at(std::make_pair(file, rank)).get_contents()->get_symbol() << " ";
            } else {
                out << "*" << " ";
            }
        }
        out << std::endl;
    }

    out << "\n     ";
    // Print out the bottom letters:
    for (char file = 'A'; file <= 'H'; ++file) {
        out << file << " ";
    }
    out << std::endl;
    out << "\n";
}

Chess::Square& Chess::Board::operator[](std::pair<char, int> location) {
    char converted_file = location.first - 'A';
    int converted_rank = location.second - 1;

    if (converted_file < 0 or converted_file > 7 or
        converted_rank < 0 or converted_rank > 7) {
        return null_square;
    } else {
        return board[converted_file][converted_rank];
    }
}

std::vector<Chess::Square>& Chess::Board::operator[](char file) {
    return board[file - 'A'];
}

std::vector<Chess::Square>& Chess::Board::at(char file) {
    int num_file = file - '@';
    return board.at(num_file);
}

Chess::Square& Chess::Board::at(std::pair<char, int> location) {
    return (*this)[location];
}