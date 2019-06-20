
#include <parser.hpp>
#include <program.hpp>
#include <tokenizer.hpp>

#include <GAlloc.hpp>
#include <Logger.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

inline bool process_file(char* p) {
  FILE* f = fopen(p, "rb");
  tcnaf::Program program;
  if(f != NULL) {
    // Find size of input file.
    fseek(f, 0, SEEK_END);
    size_t fs = ftell(f);
    fseek(f, 0, SEEK_SET);
    // Read file into C string.
    char* input = (char*)malloc(fs + 1);
    fread(input, sizeof(char), fs, f);
    input[fs] = '\0';
    fclose(f);
    // File is loaded in mem and ready to be processed.
    ssce::logi("Processing file `%s`(%zu bytes)...", p, fs);
    // Start compilation.
    void* parser = tcnaf::parser_alloc();
    tcnaf::tokenize(input, fs, parser, &program);
    tcnaf::parser_parse(parser, 0, (tcnaf::TokenData){}, &program);
    // Clean up.
    tcnaf::parser_free(parser);
    free(input);
    // Compile into binary.
    if(program.isValid()) {
      FILE* b = fopen("output.tcnaf", "wb");
      char* bin = program.compile();
      fwrite(bin, std::strlen(bin), 1, b);
      free(bin);
      fclose(b);
    }
    else {
      ssce::logf("Could not compile program %s!", program.getName());
    }
    return true;
  }
  ssce::loge("Could not open file: `%s`!", p);
  return false;
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