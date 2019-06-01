#include <tokenizer.hpp>

#include <Logger.hpp>

#include <cstddef>

namespace tcnaf {
bool tokenize(char* input) {
  // Tokenizer 'state'.
  const char* pos = input;
  const char* marked_pos = NULL;

  // Posix captures storage definitions.
  /*!maxnmatch:re2c*/
  size_t yynmatch;
  const char* yypmatch[YYMAXNMATCH * 2];
  const char* tag1;
  const char* tag2;
  const char* tag3;

  // Infinite loop.
  while(true) {
    /*!re2c
      re2c:define:YYCTYPE = char;
      re2c:define:YYCURSOR = pos;
      re2c:define:YYMARKER = marked_pos;
      re2c:tags:prefix = tag;
      re2c:yyfill:enable = 0;
      re2c:flags:utf-8 = 1;

      null = '\x00';
      int = [0-9]+;
      num = [0-9]* '.'? [0-9]+;
      ws = [ \t\n\r];

      * {
        ssce::loge("Input error!");
        return false;
      }
      ws+ {
        ssce::logw("Skipping whitespace...");
        continue;
      }
      (int) ws* '+' ws* (int) {
        ssce::logd("PLS('%s', '%s', '%s', '%s', '%s', '%s')", yypmatch[0], yypmatch[1], yypmatch[2], yypmatch[3], yypmatch[4], yypmatch[5]);
        continue;
      }
      (int) ws* '-' ws* (int) {
        ssce::logd("MIN('%s', '%s', '%s', '%s', '%s', '%s')", yypmatch[0], yypmatch[1], yypmatch[2], yypmatch[3], yypmatch[4], yypmatch[5]);
        continue;
      }
      (int) ws* '*' ws* (int) {
        ssce::logd("MUL('%s', '%s', '%s', '%s', '%s', '%s')", yypmatch[0], yypmatch[1], yypmatch[2], yypmatch[3], yypmatch[4], yypmatch[5]);
        continue;
      }
      (int) ws* '/' ws* (int) {
        ssce::logd("DIV('%s', '%s', '%s', '%s', '%s', '%s')", yypmatch[0], yypmatch[1], yypmatch[2], yypmatch[3], yypmatch[4], yypmatch[5]);
        continue;
      }
      null {
        ssce::logi("Reached end of input.");
        return true;
      }
    */
  }
  // Input string not accepted.
  return false;
}
}  // namespace tcnaf