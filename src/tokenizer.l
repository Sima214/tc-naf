#include <parser.hpp>
#include <tokenizer.hpp>

#include <GAlloc.hpp>
#include <Logger.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>

#define make_string(start, end, tmp) ({ \
  tmp = *end;                           \
  *end = '\0';                          \
  start;                                \
})

#define free_string(str, end, tmp) (*end = tmp)

#define get_index(ptr, base) (((uintptr_t)ptr) - ((uintptr_t)base))

#define PROGRESS_PRINT                                                  \
  double progress = ((double)get_index(pos, input)) / ((double)length); \
  ssce::logi("Process %05.2lf%%", progress * 100);

#define DUMP_TOKEN_INT

#define DUMP_TOKEN_FLT

#define DUMP_TOKEN_STR

#define DUMP_TOKEN_SHORT

#define MAKE_METADATA ((TokenMetadata){(unsigned int)get_index(pos, current_line), (unsigned int)line_count})

#define PARSE_INT(p, v)                                    \
  TokenData tk = {TOKEN_CONST_INTEGER, MAKE_METADATA, {}}; \
  tk.extra_data.const_int = v;                             \
  parser_parse(p, tk.token_type, tk);                      \
  PROGRESS_PRINT                                           \
  CONTINUE

#define PARSE_FLT(p, v)                                  \
  TokenData tk = {TOKEN_CONST_FLOAT, MAKE_METADATA, {}}; \
  tk.extra_data.const_flt = v;                           \
  parser_parse(p, tk.token_type, tk);                    \
  PROGRESS_PRINT                                         \
  CONTINUE

#define PARSE_STR(p, v)                         \
  TokenData tk = {TOKEN_ID, MAKE_METADATA, {}}; \
  tk.extra_data.const_str = v;                  \
  parser_parse(p, tk.token_type, tk);           \
  delete v;                                     \
  PROGRESS_PRINT                                \
  CONTINUE

#define PARSE(p, token)                              \
  TokenData tk = {TOKEN_##token, MAKE_METADATA, {}}; \
  parser_parse(p, tk.token_type, tk);                \
  PROGRESS_PRINT                                     \
  CONTINUE

#define CONTINUE  \
  prev_pos = pos; \
  continue;

namespace tcnaf {
bool tokenize(char* input, size_t length, void* parser) {
  // Tokenizer 'state'.
  char* pos = input;
  char pos_tmp = '\0';
  char* marked_pos = NULL;
  char* prev_pos = input;
  size_t line_count = 1;
  char* current_line = input;

  // Infinite loop.
  while(true) {
    /*!re2c
      re2c:define:YYCTYPE = uint8_t;
      re2c:define:YYCURSOR = pos;
      re2c:define:YYMARKER = marked_pos;
      re2c:tags:prefix = tag;
      re2c:yyfill:enable = 0;
      re2c:flags:utf-8 = 1;
      re2c:indent:top = 3;
      re2c:indent:string="  ";

      null = '\x00';
      ws = [ \t\r];
      newline = '\n';
      comment = "//" [^\n]*;
      integer = [0-9]+;
      float = [0-9]* '.' [0-9]+;
      numerical = integer | float;
      id = [a-zA-Z] ([a-zA-Z0-9_])*;

      * {
        ssce::loge("Invalid character `%c` at %zu:%zu!", *pos, line_count, get_index(pos, current_line));
        return false;
      }
      newline {
        line_count++;
        current_line = pos;
        CONTINUE;
      }
      comment {
        CONTINUE;
      }
      ws+ {
        CONTINUE;
      }
      "class" {
        PARSE(parser, CLASS);
      }
      "public static void" {
        PARSE(parser, FUNCTION);
      }
      "(" {
        PARSE(parser, PRNTH_OPEN);
      }
      ")" {
        PARSE(parser, PRNTH_CLOSE);
      }
      "{" {
        PARSE(parser, BLOCK_START);
      }
      "}" {
        PARSE(parser, BLOCK_END);
      }
      "," {
        PARSE(parser, COMMA_SEP);
      }
      ";" {
        PARSE(parser, STATEMENT_END);
      }
      "int" {
        PARSE(parser, TYPE_INTEGER);
      }
      "float" {
        PARSE(parser, TYPE_FLOAT);
      }
      "==" {
        PARSE(parser, OP_EQUAL);
      }
      "!=" {
        PARSE(parser, OP_NOT_EQUAL);
      }
      ">=" {
        PARSE(parser, OP_GREATER_EQUAL);
      }
      "<=" {
        PARSE(parser, OP_LESS_EQUAL);
      }
      ">" {
        PARSE(parser, OP_GREATER);
      }
      "<" {
        PARSE(parser, OP_LESS);
      }
      "=" {
        PARSE(parser, ASSIGNMENT);
      }
      "+" {
        PARSE(parser, OP_ADD);
      }
      "-" {
        PARSE(parser, OP_SUB);
      }
      "*" {
        PARSE(parser, OP_MUL);
      }
      "/" {
        PARSE(parser, OP_DIV);
      }
      "while" {
        PARSE(parser, WHILE);
      }
      "for" {
        PARSE(parser, FOR);
      }
      "if" {
        PARSE(parser, IF);
      }
      "else" {
        PARSE(parser, ELSE);
      }
      float {
        char* str = make_string(prev_pos, pos, pos_tmp);
        double value = std::atof(str);
        free_string(str, pos, pos_tmp);
        PARSE_FLT(parser, value);
      }
      integer {
        char* str = make_string(prev_pos, pos, pos_tmp);
        int64_t value = std::strtoll(str, NULL, 10);
        free_string(str, pos, pos_tmp);
        PARSE_INT(parser, value);
      }
      id {
        char* id = make_string(prev_pos, pos, pos_tmp);
        std::string* value = new std::string(id);
        free_string(id, pos, pos_tmp);
        PARSE_STR(parser, value);
      }
      null {
        ssce::logi("Lexical Analysis OK!");
        return true;
      }
    */
  }
  // Input string not accepted.
  return false;
}
}  // namespace tcnaf