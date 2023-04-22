#ifndef __EXIT_WITH_ERROR_H__
#define __EXIT_WITH_ERROR_H__ 1

#include "all_imports.h"

int exit_with_error(char *error_message) {
  fprintf(stderr, "%s", error_message);
#ifdef _WIN32
  printf("WSACleanup() called.\n");
  WSACleanup();
#endif
  return EXIT_FAILURE;
}

#endif // __EXIT_WITH_ERROR_H__