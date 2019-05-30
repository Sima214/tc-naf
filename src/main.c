#include <Logger.h>

#include <stdlib.h>

static int process_file(char* p) {
  logger_logi("Processing %s...", p);
  return 1;
}

int main(int argc, char* argv[]) {
  if(argc == 1) {
    logger_logf("No source files specified!");
  }
  for(int i = 1; i < argc; i++) {
    // Current source file.
    char* current_source_path = argv[i];
    int ok = process_file(current_source_path);
    if(!ok) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}