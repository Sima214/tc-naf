#ifndef TCNAF_TOKEN_HPP
#define TCNAF_TOKEN_HPP

#include <Macros.h>

#include <cstddef>
#include <cstdint>
#include <string>

namespace tcnaf {

#include "/home/sima/Storage/projects/C/tc-naf/build/parser.h"

struct MARK_PACKED TokenMetadata {
  unsigned int column:24;
  u_int32_t line;
};

union TokenExtra {
  int64_t const_int;
  double const_flt;
  std::string* const_str;
};


struct TokenData {
  TokenType token_type:8;
  TokenMetadata meta_data;
  TokenExtra extra_data;
};

bool tokenize(char* input, size_t length, void* parser);
}

#endif /*TCNAF_TOKEN_HPP*/
