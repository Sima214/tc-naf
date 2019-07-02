#ifndef TCNAF_AST_HPP
#define TCNAF_AST_HPP

#include <GAlloc.hpp>
#include <Logger.hpp>

#include <list>
#include <string>
#include <unordered_map>
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

class VariableStore;

class BaseStatement {
 public:
  virtual ~BaseStatement() {}
  virtual std::string* compile(int level) = 0;
  virtual bool validate(VariableStore&) = 0;
};

class BaseExpression : public BaseStatement {
 public:
  virtual ReturnType getReturnType() = 0;
};

struct Variable {
  ReturnType type;
  int uuid;
  std::string name;
  BaseStatement* declarator;
};

class VariableStore {
 public:
  int next_uuid = 0;
  std::unordered_map<std::string, std::list<Variable>> store;
  Variable getVariable(std::string name) {
    std::list<Variable>& list = store[name];
    if(!list.empty()) {
      return list.front();
    }
    return (Variable){RETTYPE_VOID, -1, "", NULL};
  }
  Variable addVariable(ReturnType type, std::string name, BaseStatement* decl) {
    Variable new_var = {type, next_uuid, name, decl};
    next_uuid++;
    std::list<Variable>& list = store[name];
    // TODO: same level multiple declaration?
    list.push_front(new_var);
    return new_var;
  }
  void removeVariable(Variable v) {
    std::list<Variable>& list = store[v.name];
    if(list.empty()) {
      ssce::logf("Variable store empty!");
    }
    if(list.front().uuid != v.uuid) {
      ssce::logf("Can only remove from top!");
    }
    list.pop_front();
  }
  std::size_t usage() {
    return 4;
  }
};

inline std::string getIdent(int level) {
  std::string ident(level * 2, ' ');
  return ident;
}

}  // namespace tcnaf

#endif /*TCNAF_AST_HPP*/