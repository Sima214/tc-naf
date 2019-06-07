#ifndef TCNAF_TOKEN_HPP
#define TCNAF_TOKEN_HPP

#include <cstddef>

namespace tcnaf {

#include "/home/sima/Storage/projects/C/tc-naf/build/parser.h"

struct TokenData {
  int token_type;
};

bool tokenize(char* input, size_t length, void* parser);

}

#endif /*TCNAF_TOKEN_HPP*/
