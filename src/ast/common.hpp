#ifndef TCNAF_AST_HPP
#define TCNAF_AST_HPP

#include <list>
#include <map>
#include <string>
#include <vector>

namespace tcnaf {

/**
 * Return type for expressions during semantic analysis.
 */
enum ReturnType {
  RETTYPE_VOID = 0,
  RETTYPE_BOOL = 1,
  RETTYPE_INT = 2,
  RETTYPE_FLOAT = 3,
  RETTYPE_UNKNOWN = -1
};

enum BoolOp {
  BOOP_EQ,
  BOOP_L,
  BOOP_G,
  BOOP_LE,
  BOOP_GE,
  BOOP_NEQ
};

class BaseStatement {
 public:
  virtual ~BaseStatement() {}
  virtual std::string* compile(int level) = 0;
};

class BaseExpression : public BaseStatement {
 public:
  virtual ReturnType getReturnType() = 0;
};

struct Variable {
  ReturnType type;
  std::string name;
  BaseStatement* declarator;
};

class VariableStore {
 public:
  std::map<std::string, std::list<Variable>> store;
};

inline std::string getIdent(int level) {
  std::string ident(level * 2, ' ');
  return ident;
}

}  // namespace tcnaf

#endif /*TCNAF_AST_HPP*/