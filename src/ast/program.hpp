#ifndef TCNAF_AST_ROOT_HPP
#define TCNAF_AST_ROOT_HPP

#include <GAlloc.hpp>

#include <cstddef>
#include <string>

namespace tcnaf {

class Program {
 private:
  std::string* name;
  bool valid;

 public:
  Program(std::string* id) :
    name(id),
    valid(true){
      /* NOP */
    };

  Program() :
    name(NULL),
    valid(false){
      /* NOP */
    };

  // TODO: Copy

  ~Program() {
    delete name;
  }

  void setName(std::string* id) {
    name = id;
    valid = true;
  }

  std::string* getName() {
    return name;
  }

  /**
   * Returns true if the program is valid and ready to get compiled into binary.
   */
  bool isValid() {
    return valid;
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