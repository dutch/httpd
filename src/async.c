#include <config.h>
#include <stdlib.h>
#include <string.h>
#include "async.h"
#include "private.h"

extern void async__init_epoll(async__vtable_t *);
extern void async__init_kqueue(async__vtable_t *);

typedef struct async__backend
{
  char *name;
  void (*init)(async__vtable_t *);
} async__backend_t;

static const async__backend_t
async__backends[] = {
#ifdef HAVE_SYS_EPOLL_H
  { "epoll", async__init_epoll },
#endif
#ifdef HAVE_SYS_EVENT_H
  { "kqueue", async__init_kqueue },
#endif
  { NULL, NULL }
};

async__vtable_t async__vtable;

void
async_init(void)
{
  char *backend_str;
  int i;

  backend_str = getenv("ASYNC_BACKEND");

  if (!backend_str || *backend_str == '\0')
  	backend_str = async__backends[0].name;

  for (i = 0; async__backends[i].name; ++i)
    if (strcmp(async__backends[i].name, backend_str) == 0)
      async__backends[i].init(&async__vtable);
}

void
async_read(int fd, void *buf, size_t len, void (*f)(int, void *), void *p)
{
  async__vtable.read(fd, buf, len, f, p);
}

void
async_loop(void)
{
  async__vtable.loop();
}
