#include "expressions.hpp"
#include "program.hpp"
#include "statements.hpp"

#include <GAlloc.hpp>
#include <Logger.hpp>
#include <Macros.h>

#include <cstdlib>
#include <string>

std::string* tcnaf::Program::compile() {
  std::string* ret = new std::string();
  for(auto const& func : func_list) {
    ret->append("Define function ");
    ret->append(name);
    ret->append("::");
    ret->append(func.first);
    StatementList* block = func.second;
    std::string* block_str = block->compile(1);
    ret->append("\n");
    ret->append(*block_str);
    delete block_str;
  }
  return ret;
};

std::string* tcnaf::StatementList::compile(int level) {
  std::string* str = new std::string(tcnaf::getIdent(level));
  // TODO: stack allocation.
  str->append("Block start:\n");
  for(std::vector<BaseStatement*>::iterator it = list.begin(); it != list.end(); it++) {
    BaseStatement* c = *it;
    std::string* cs = c->compile(level + 1);
    str->append(*cs);
    str->append("\n");
    delete cs;
  }
  str->append(tcnaf::getIdent(level));
  str->append("end");
  return str;
}

std::string* tcnaf::StatementDeclare::compile(int level) {
  std::string* str = new std::string(tcnaf::getIdent(level));
  str->append("Allocating ");
  if(type == RETTYPE_INT) {
    str->append("integers: [");
  } else if(type == RETTYPE_FLOAT) {
    str->append("floats: [");
  } else {
    ssce::logf("StatementDeclare invalid type!");
  }
  for(std::vector<std::string>::iterator it = list->begin(); it != list->end(); it++) {
    // TODO: add var address info.
    str->append(*it);
    if((it + 1) != list->end()) {
      str->append(", ");
    }
  }
  str->append("]");
  return str;
}

std::string* tcnaf::StatementFunction::compile(int level) {
  std::string* es = expression->compile(0);
  std::string* str = new std::string(tcnaf::getIdent(level));
  str->append("Calling <");
  str->append(name);
  str->append(">(");
  str->append(*es);
  str->append(")");
  delete es;
  return str;
}

std::string* tcnaf::StatementIfElse::compile(int level) {
  std::string ident = tcnaf::getIdent(level);
  std::string* str = new std::string(ident);
  std::string* ls = expr_log->compile(0);
  str->append("If <");
  str->append(*ls);
  str->append(">:\n");
  delete ls;
  std::string* is = stm_if->compile(level + 1);
  str->append(*is);
  str->append("\n");
  delete is;
  if(stm_else != NULL) {
    str->append(ident);
    str->append("else:\n");
    std::string* es = stm_else->compile(level + 1);
    str->append(*es);
    str->append("\n");
    delete es;
  }
  str->append(ident);
  str->append("end_if");
  return str;
}

std::string* tcnaf::StatementForLoop::compile(int level) {
  std::string ident = tcnaf::getIdent(level);
  std::string* str = new std::string(ident);
  if(init != NULL) {
    std::string* is = init->compile(level);
    is->append("\n");
    str->insert(0, *is);
    delete is;
  }
  if(condition != NULL) {
    std::string* cs = condition->compile(0);
    str->append("For <");
    str->append(*cs);
    str->append(">:\n");
    delete cs;
  } else {
    str->append("Forever:\n");
  }
  std::string* bs = block->compile(level + 1);
  str->append(*bs);
  str->append("\n");
  delete bs;
  if(increment != NULL) {
    std::string* ls = increment->compile(level + 1);
    str->append(*ls);
    str->append("\n");
    delete ls;
  }
  str->append(ident);
  str->append("end_for");
  return str;
}

std::string* tcnaf::StatementWhileLoop::compile(int level) {
  std::string ident = tcnaf::getIdent(level);
  std::string* str = new std::string(ident);
  str->append("While <");
  std::string* cs = condition->compile(0);
  str->append(*cs);
  delete cs;
  str->append(">:\n");
  std::string* bs = block->compile(level + 1);
  str->append(*bs);
  str->append("\n");
  str->append(ident);
  str->append("end_while");
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionConstInteger::compile(MARK_UNUSED int level) {
  return new std::string(std::to_string(value));
}

std::string* tcnaf::ExpressionConstFloat::compile(MARK_UNUSED int level) {
  return new std::string(std::to_string(value));
}

std::string* tcnaf::ExpressionVariable::compile(MARK_UNUSED int level) {
  std::string* str = new std::string();
  str->append("[*");
  str->append(name);
  str->append("]");
  return str;
}

std::string* tcnaf::ExpressionNegate::compile(MARK_UNUSED int level) {
  std::string* str = expr->compile(0);
  str->insert(0, "neg(");
  str->append(")");
  return str;
}

std::string* tcnaf::ExpressionAdd::compile(MARK_UNUSED int level) {
  std::string* as = expr_a->compile(0);
  std::string* bs = expr_b->compile(0);
  std::string* str = new std::string("add(");
  str->append(*as);
  str->append(", ");
  str->append(*bs);
  str->append(")");
  delete as;
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionSub::compile(MARK_UNUSED int level) {
  std::string* as = expr_a->compile(0);
  std::string* bs = expr_b->compile(0);
  std::string* str = new std::string("sub(");
  str->append(*as);
  str->append(", ");
  str->append(*bs);
  str->append(")");
  delete as;
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionMul::compile(MARK_UNUSED int level) {
  std::string* as = expr_a->compile(0);
  std::string* bs = expr_b->compile(0);
  std::string* str = new std::string("mul(");
  str->append(*as);
  str->append(", ");
  str->append(*bs);
  str->append(")");
  delete as;
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionDiv::compile(MARK_UNUSED int level) {
  std::string* as = expr_a->compile(0);
  std::string* bs = expr_b->compile(0);
  std::string* str = new std::string("div(");
  str->append(*as);
  str->append(", ");
  str->append(*bs);
  str->append(")");
  delete as;
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionLogical::compile(MARK_UNUSED int level) {
  std::string* as = expr_a->compile(0);
  std::string* bs = expr_b->compile(0);
  std::string* str = new std::string();
  switch(operation) {
    case BOOP_EQ:
      str->append("cmp_eq(");
      break;
    case BOOP_L:
      str->append("cmp_l(");
      break;
    case BOOP_G:
      str->append("cmp_g(");

      break;
    case BOOP_LE:
      str->append("cmp_le(");
      break;
    case BOOP_GE:
      str->append("cmp_ge(");
      break;
    case BOOP_NEQ:
      str->append("cmp_neq(");
      break;
    default:
      ssce::logf("Invalid BOOP!");
      break;
  }
  str->append(*as);
  str->append(", ");
  str->append(*bs);
  str->append(")");
  delete as;
  delete bs;
  return str;
}

std::string* tcnaf::ExpressionAssignment::compile(int level) {
  std::string* str = new std::string(tcnaf::getIdent(level));
  std::string* es = expr->compile(0);
  // TODO: type casts.
  str->append(name);
  str->append("<-");
  str->append(*es);
  delete es;
  return str;
}