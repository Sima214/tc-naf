#include "program.hpp"

#include <GAlloc.hpp>

#include <cstdlib>
#include <cstring>

char* tcnaf::Program::compile() {
  char* ret = (char*) malloc(strlen(name) + 1);
  strcpy(ret, name);
  return ret;
};