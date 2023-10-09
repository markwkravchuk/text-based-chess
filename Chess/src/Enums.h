#ifndef CHESS_ENUMS_H
#define CHESS_ENUMS_H

namespace Chess {

    enum class Color {
        WHITE, BLACK
    };

    enum class PieceType {
        KING, QUEEN, PAWN, ROOK, BISHOP, KNIGHT
    };

    enum class MoveType {
        INVALID, STANDARD, EN_PASSANT_LEFT, EN_PASSANT_RIGHT, PROMOTION, CASTLE_SHORT, CASTLE_LONG
    };

    enum class Direction {
        N, NE, E, SE, S, SW, W, NW, NONE
    };

    enum class SquareAction {
        ADD_TO_ATTACKING, ADD_TO_POSSIBLE_SQUARES, ADD_TO_BOTH
    };
}

#endif //CHESS_ENUMS_H
