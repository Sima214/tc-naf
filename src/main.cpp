#include <GAlloc.hpp>
#include <Logger.hpp>

#include <cstdlib>
#include <fstream>

inline bool process_file(char* p) {
  std::ifstream f(p);
  if(f.good()) {
    ssce::logi("Processing file `%s`...", p);
    return true;
  } else {
    ssce::loge("Could not open file: `%s`!", p);
    return false;
  }
}

int main(int argc, char* argv[]) {
  if(argc == 1) {
    ssce::loge("No source files specified!");
    return EXIT_FAILURE;
  }
  for(int i = 1; i < argc; i++) {
    // Current source file.
    char* current_source_path = argv[i];
    bool ok = process_file(current_source_path);
    if(!ok) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}