#ifndef TCNAF_TOKEN_HPP
#define TCNAF_TOKEN_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace tcnaf {

#include "/home/sima/Storage/projects/C/tc-naf/build/parser.h"

union TokenExtra {
  int64_t const_int;
  double const_flt;
  std::string* const_str;
};


struct TokenData {
  TokenType token_type:8;
  TokenExtra extra_data;
};

bool tokenize(char* input, size_t length, void* parser);
}

#endif /*TCNAF_TOKEN_HPP*/
