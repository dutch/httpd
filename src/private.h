#ifndef PRIVATE_HEADER
#define PRIVATE_HEADER

#include <stdlib.h>

typedef struct async__vtable
{
  void (*read)(int, void *, size_t, void (*)(int, void *), void *);
  void (*loop)(void);
} async__vtable_t;

#endif
