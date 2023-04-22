#ifndef __ERASE_FROM_MEMORY_H__
#define __ERASE_FROM_MEMORY_H__ 1

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <string.h>

void *erase_from_memory(void *pointer, size_t size_data,
                        size_t size_to_remove) {
#ifdef __STDC_LIB_EXT1__
  memset_s(pointer, size_data, 0, size_to_remove);
#else
  if (size_to_remove > size_data)
    size_to_remove = size_data;
  volatile unsigned char *p = (volatile unsigned char *)pointer;
  while (size_to_remove--) {
    *p++ = 0;
  }
#endif
  return pointer;
}

#endif // __ERASE_FROM_MEMORY_H__