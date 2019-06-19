#include <Macros.h>
C_DECLS_START
#include <GAlloc.h>
C_DECLS_END

#include <cstdlib>
#include <new>

/**
 * Override new/delete operators.
 */

void* operator new(std::size_t count) {
  return je_malloc(count);
}

void operator delete(void* ptr) {
  je_free(ptr);
}

void operator delete(void* ptr, MARK_UNUSED std::size_t count) {
  je_free(ptr);
}