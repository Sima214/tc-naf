#ifndef TCNAF_AST_STATEMENT_HPP
#define TCNAF_AST_STATEMENT_HPP

#include <common.hpp>

#include <string>
#include <vector>

#include <Logger.hpp>

namespace tcnaf {

class StatementDeclare : public BaseStatement {
 public:
  ReturnType type;
  std::vector<std::string>* list;
  StatementDeclare(ReturnType t, std::vector<std::string>* l) :
    type(t),
    list(l) {}
  virtual std::string* compile(int level);
};

class StatementFunction : public BaseStatement {
 public:
  std::string name;
  BaseExpression* expression;
  StatementFunction(const char* id, BaseExpression* expr) :
    name(id),
    expression(expr) {}
  virtual std::string* compile(int level);
};

class StatementIfElse : public BaseStatement {
 public:
  BaseExpression* expr_log;
  BaseStatement* stm_if;
  BaseStatement* stm_else;
  StatementIfElse(BaseExpression* l, BaseStatement* i, BaseStatement* e) :
    expr_log(l),
    stm_if(i),
    stm_else(e) {}
  virtual std::string* compile(int level);
};

class StatementForLoop : public BaseStatement {
 public:
  BaseExpression* init;
  BaseExpression* condition;
  BaseExpression* increment;
  BaseStatement* block;
  StatementForLoop(BaseExpression* i, BaseExpression* c, BaseExpression* l, BaseStatement* b) :
    init(i),
    condition(c),
    increment(l),
    block(b) {}
  virtual std::string* compile(int level);
};

class StatementWhileLoop : public BaseStatement {
 public:
  BaseExpression* condition;
  BaseStatement* block;
  StatementWhileLoop(BaseExpression* c, BaseStatement* b) :
    condition(c),
    block(b) {}
  virtual std::string* compile(int level);
};

class StatementList : public BaseStatement {
 public:
  std::vector<BaseStatement*> list;
  void appendStatement(BaseStatement* e) {
    // Finalize.
    list.push_back(e);
  };
  virtual std::string* compile(int level);
};

}  // namespace tcnaf

#endif /*TCNAF_AST_STATEMENT_HPP*/