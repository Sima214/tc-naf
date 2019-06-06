#ifndef TCNAF_PARSER_HPP
#define TCNAF_PARSER_HPP

#include <tokenizer.hpp>

namespace tcnaf {
void* parser_alloc();
void parser_parse(
  void* yyp, /* The parser */
  int yymajor, /* The major token code number */
  TokenData yyminor /* The value for the token */
  /* Optional %extra_argument parameter */
);
void parser_free(void* p);
}  // namespace tcnaf

#endif /*TCNAF_PARSER_HPP*/