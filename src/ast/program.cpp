#include "program.hpp"

#include <GAlloc.hpp>

#include <cstdlib>
#include <cstring>

char* tcnaf::Program::compile() {
  char* ret = (char*) malloc(name->length() + 1);
  strcpy(ret, name->c_str());
  return ret;
};