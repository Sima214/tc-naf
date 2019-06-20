#include "common.hpp"
#include "expressions.hpp"
#include "program.hpp"
#include "statements.hpp"

#include <GAlloc.hpp>
#include <Logger.hpp>
#include <Macros.h>

#include <cstdlib>
#include <string>

bool tcnaf::ExpressionConstInteger::validate(MARK_UNUSED tcnaf::VariableStore& store) {
  return true;
}

bool tcnaf::ExpressionConstFloat::validate(MARK_UNUSED tcnaf::VariableStore& store) {
  return true;
}

bool tcnaf::ExpressionVariable::validate(tcnaf::VariableStore& store) {
  tcnaf::Variable ref = store.getVariable(reference.name);
  if(ref.type == RETTYPE_INT || ref.type == RETTYPE_FLOAT) {
    reference = ref;
    return true;
  } else {
    ssce::loge("Could not find variable `%s`", reference.name.c_str());
    return false;
  }
}

bool tcnaf::ExpressionNegate::validate(tcnaf::VariableStore& store) {
  return expr->validate(store);
}

bool tcnaf::ExpressionAdd::validate(tcnaf::VariableStore& store) {
  bool a = expr_a->validate(store);
  bool b = expr_b->validate(store);
  return a && b;
}

bool tcnaf::ExpressionSub::validate(tcnaf::VariableStore& store) {
  bool a = expr_a->validate(store);
  bool b = expr_b->validate(store);
  return a && b;
}

bool tcnaf::ExpressionMul::validate(tcnaf::VariableStore& store) {
  bool a = expr_a->validate(store);
  bool b = expr_b->validate(store);
  return a && b;
}

bool tcnaf::ExpressionDiv::validate(tcnaf::VariableStore& store) {
  bool a = expr_a->validate(store);
  bool b = expr_b->validate(store);
  return a && b;
}

bool tcnaf::ExpressionLogical::validate(tcnaf::VariableStore& store) {
  bool a = expr_a->validate(store);
  bool b = expr_b->validate(store);
  // TODO: casting?
  return a && b;
}

bool tcnaf::ExpressionAssignment::validate(tcnaf::VariableStore& store) {
  tcnaf::Variable ref = store.getVariable(reference.name);
  if(ref.type == RETTYPE_INT || ref.type == RETTYPE_FLOAT) {
    reference = ref;
  } else {
    ssce::loge("Could not find variable `%s`", reference.name.c_str());
    return false;
  }
  bool expr_ok = expr->validate(store);
  if(expr_ok) {
    ReturnType expr_type = expr->getReturnType();
    if(expr_type != RETTYPE_INT && expr_type != RETTYPE_FLOAT) {
      ssce::loge("Assignment expression neither integer nor float!");
      return false;
    }
    if(expr_type != ref.type) {
      ssce::logw("Implicit casting at assignment expression.");
    }
  }
  return expr_ok;
}

bool tcnaf::StatementFunction::validate(tcnaf::VariableStore& store) {
  if(name.compare("println") != 0) {
    ssce::loge("Invalid function call `%s`!", name.c_str());
    return false;
  }
  bool expr_ok = expression->validate(store);
  if(expr_ok) {
    ReturnType expr_type = expression->getReturnType();
    if(expr_type != RETTYPE_INT && expr_type != RETTYPE_FLOAT && expr_type != RETTYPE_BOOL) {
      ssce::loge("Invalid parameters at function call `%s`!", name.c_str());
      return false;
    }
  }
  return expr_ok;
}

bool tcnaf::StatementIfElse::validate(tcnaf::VariableStore& store) {
  bool expr_log_ok = expr_log->validate(store);
  if(expr_log_ok) {
    bool stm_if_ok = stm_if->validate(store);
    if(stm_if_ok) {
      if(stm_else != NULL) {
        return stm_else->validate(store);
      } else {
        return true;
      }
    }
    return false;
  }
  return false;
}

bool tcnaf::StatementForLoop::validate(tcnaf::VariableStore& store) {
  bool init_ok = init == NULL || init->validate(store);
  bool condition_ok = condition == NULL || condition->validate(store);
  bool increment_ok = increment == NULL || increment->validate(store);
  bool block_ok = block == NULL || block->validate(store);
  return init_ok && condition_ok && increment_ok && block_ok;
}

bool tcnaf::StatementWhileLoop::validate(tcnaf::VariableStore& store) {
  return condition->validate(store) && block->validate(store);
}

bool tcnaf::StatementList::validate(tcnaf::VariableStore& store) {
  for(std::vector<BaseStatement*>::iterator it = list.begin(); it != list.end(); it++) {
    BaseStatement* c = *it;
    bool ok = c->validate(store);
    if(!ok) {
      return false;
    }
  }
  // Pop stack.
  for(std::vector<BaseStatement*>::iterator it = list.end(); it != list.begin(); it--) {
    BaseStatement* c = *it;
    if(c == NULL) {
      continue;
    }
    //if(StatementDeclare* d = dynamic_cast<StatementDeclare*>(c)) {
    //  d->deallocate(store);
    //}
  }
  return true;
}

void tcnaf::StatementDeclare::deallocate(tcnaf::VariableStore& store) {
  for(std::vector<tcnaf::Variable>::iterator it = references.end(); it != references.begin(); it--) {
    Variable r = *it;
    store.removeVariable(r);
  }
}

bool tcnaf::StatementDeclare::validate(tcnaf::VariableStore& store) {
  for(std::vector<std::string>::iterator it = list->begin(); it != list->end(); it++) {
    Variable r = store.addVariable(type, *it, this);
    references.push_back(r);
  }
  return true;
}