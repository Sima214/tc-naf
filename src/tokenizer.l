#include <parser.hpp>
#include <tokenizer.hpp>

#include <Logger.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#define make_string(start, end, tmp) ({ \
  tmp = *end;                           \
  *end = '\0';                          \
  start;                                \
})

#define free_string(str, end, tmp) (*end = tmp)

#define get_index(ptr, base) (((uintptr_t)ptr) - ((uintptr_t)base))

#define PROGRESS_UPDATE double progress = ((double)get_index(pos, input)) / ((double)length);

#define PROGRESS_PRINT ssce::logi("Process %05.2lf%%", progress * 100);

#define RULE_END_TOKEN \
  PROGRESS_UPDATE      \
  PROGRESS_PRINT       \
  prev_pos = pos;      \
  continue;

#define RULE_END   \
  PROGRESS_UPDATE; \
  prev_pos = pos;  \
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
        RULE_END;
      }
      comment {
        RULE_END;
      }
      ws+ {
        RULE_END;
      }
      "class" {
        ssce::logd("PROGRAM");
        RULE_END_TOKEN;
      }
      "public static void" {
        ssce::logd("FUNCTION");
        RULE_END_TOKEN;
      }
      "(" {
        ssce::logd("PRNTH_OPEN");
        RULE_END_TOKEN;
      }
      ")" {
        ssce::logd("PRNTH_CLOSE");
        RULE_END_TOKEN;
      }
      "{" {
        ssce::logd("BLOCK_START");
        RULE_END_TOKEN;
      }
      "}" {
        ssce::logd("BLOCK_END");
        RULE_END_TOKEN;
      }
      "," {
        ssce::logd("SEPARATOR");
        RULE_END_TOKEN;
      }
      ";" {
        ssce::logd("EXPR_END");
        RULE_END_TOKEN;
      }
      "int" {
        ssce::logd("TYPE_INT");
        RULE_END_TOKEN;
      }
      "float" {
        ssce::logd("TYPE_FLOAT");
        RULE_END_TOKEN;
      }
      "==" {
        ssce::logd("CMP_EQ");
        RULE_END_TOKEN;
      }
      "!=" {
        ssce::logd("CMP_NEQ");
        RULE_END_TOKEN;
      }
      ">=" {
        ssce::logd("CMP_GREATER_EQ");
        RULE_END_TOKEN;
      }
      "<=" {
        ssce::logd("CMP_LESS_EQ");
        RULE_END_TOKEN;
      }
      ">" {
        ssce::logd("CMP_GREATER");
        RULE_END_TOKEN;
      }
      "<" {
        ssce::logd("CMP_LESS");
        RULE_END_TOKEN;
      }
      "=" {
        ssce::logd("OP_ASSIGNMENT");
        RULE_END_TOKEN;
      }
      "+" {
        ssce::logd("OP_ADDITION");
        RULE_END_TOKEN;
      }
      "-" {
        ssce::logd("OP_SUBTRACTION");
        RULE_END_TOKEN;
      }
      "*" {
        ssce::logd("OP_MULTIPLICATION");
        RULE_END_TOKEN;
      }
      "/" {
        ssce::logd("OP_DIVISION");
        RULE_END_TOKEN;
      }
      "while" {
        ssce::logd("BRANCH_WHILE");
        RULE_END_TOKEN;
      }
      "for" {
        ssce::logd("BRANCH_FOR");
        RULE_END_TOKEN;
      }
      "if" {
        ssce::logd("BRANCH_IF");
        RULE_END_TOKEN;
      }
      "else" {
        ssce::logd("BRANCH_ELSE");
        RULE_END_TOKEN;
      }
      float {
        char* str = make_string(prev_pos, pos, pos_tmp);
        double value = std::atof(str);
        ssce::logd("float<%lf>", value);
        free_string(str, pos, pos_tmp);
        RULE_END_TOKEN;
      }
      integer {
        char* str = make_string(prev_pos, pos, pos_tmp);
        int64_t value = std::strtoll(str, NULL, 10);
        ssce::logd("int<%lld>", value);
        free_string(str, pos, pos_tmp);
        RULE_END_TOKEN;
      }
      id {
        char* id = make_string(prev_pos, pos, pos_tmp);
        ssce::logd("id<%s>", id);
        free_string(id, pos, pos_tmp);
        RULE_END_TOKEN;
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