//
// Created by mk on 8/21/23.
//

#ifndef CHESS_HELPERS_H
#define CHESS_HELPERS_H

#include <set>
#include "Square.h"

namespace Chess {
    bool in(Square& value, std::set<Square&> iterable);
    bool is_overlapping(std::vector<Square&> a, std::vector<Square&> b);
}

#endif //CHESS_HELPERS_H
