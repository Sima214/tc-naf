#ifndef TCNAF_AST_ROOT_HPP
#define TCNAF_AST_ROOT_HPP

#include <statements.hpp>

#include <GAlloc.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>

namespace tcnaf {

class Program {
 private:
  const char* name;
  bool errored;
  std::unordered_map<std::string, StatementList*> func_list;

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
    if(!errored && name != NULL) {
      // Start semantic analysis.
      for(auto const& func : func_list) {
        StatementList* block = func.second;
        VariableStore store;
        if(!block->validate(store)) {
          ssce::logf("Semantic analysis failed!");
        }
      }
      return true;
    } else {
      return false;
    }
  }

  void addFunc(const char* name, StatementList* stml) {
    func_list[name] = stml;
  }

  /**
   * Returns a binary representation if this program.
   * 
   * Returned pointer should be freed by the caller.
   */
  std::string* compile();
};

}  // namespace tcnaf

#endif /*TCNAF_AST_ROOT_HPP*/