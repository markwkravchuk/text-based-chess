cmake_minimum_required(VERSION 3.22)
project(ChessSrc)

set(CMAKE_CXX_STANDARD 14)

add_library(ChessSrc Game.cpp Game.h Player.cpp Player.h Pieces/Piece.cpp Pieces/Piece.h Board.cpp Board.h Square.cpp Square.h Move.cpp Move.h Pieces/Bishop.cpp Pieces/Bishop.h Pieces/King.cpp Pieces/King.h Pieces/Knight.cpp Pieces/Knight.h Pieces/Queen.cpp Pieces/Queen.h Pieces/Rook.cpp Pieces/Rook.h Pieces/Pawn.cpp Pieces/Pawn.h Helpers.cpp Helpers.h Enums.h)

target_compile_options(ChessSrc PRIVATE -Wall -Werror)
target_include_directories(ChessSrc PUBLIC .)

add_executable(ChessGame main.cpp)
target_link_libraries(ChessGame PRIVATE ChessSrc)

target_compile_options(ChessGame PRIVATE -Wall -Werror)