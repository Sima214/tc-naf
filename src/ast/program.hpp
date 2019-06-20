#ifndef TCNAF_AST_ROOT_HPP
#define TCNAF_AST_ROOT_HPP

#include <statements.hpp>

#include <GAlloc.hpp>

#include <cstddef>
#include <string>

namespace tcnaf {

class Program {
 private:
  const char* name;
  bool errored;

 public:
  Program(const char* id) :
    name(id),
    errored(false){
      /* NOP */
    };

  Program() :
    name(NULL),
    errored(false){
      /* NOP */
    };

  // TODO: Copy

  ~Program() {
    delete name;
  }

  void setName(const char* id) {
    name = id;
  }

  const char* getName() {
    return name;
  }

  void setErrored() {
    errored = true;
  }

  /**
   * Returns true if the program is valid and ready to get compiled into a binary.
   */
  bool isValid() {
    return !errored && name != NULL;
  }

  /**
   * Returns a binary representation if this program.
   * 
   * Returned pointer should be freed by the caller.
   */
  char* compile();
};

}  // namespace tcnaf

#endif /*TCNAF_AST_ROOT_HPP*/