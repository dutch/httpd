#include <stdio.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include "../async.h"
#include "../private.h"

#define ASYNC_TYPE_READ 1

typedef struct async__read_data
{
  int fd;
  char *buf;
  size_t len;
  void (*f)(int, void *);
  void *p;
} async__read_data_t;

typedef struct async__event_data
{
  int type;
  union {
    async__read_data_t read_data;
  } payload;
} async__event_data_t;

int async__kqueuefd;

void
async__read_kqueue(int fd, void *buf, size_t len, void (*f)(int, void *), void *p)
{
  async__event_data_t *data;
  struct kevent ev;

  data = malloc(sizeof(async__read_data_t));
  data->type = ASYNC_TYPE_READ;
  data->payload.read_data.fd = fd;
  data->payload.read_data.buf = buf;
  data->payload.read_data.len = len;
  data->payload.read_data.f = f;
  data->payload.read_data.p = p;

  EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, data);

  if (kevent(async__kqueuefd, &ev, 1, NULL, 0, NULL) == -1) {
    perror("kevent");
    _exit(EXIT_FAILURE);
  }
}

void
async__loop_kqueue(void)
{
  int nevs, res;
  struct kevent evs[2];
  async__event_data_t *p;

  for (;;) {
    if ((nevs = kevent(async__kqueuefd, NULL, 0, evs, 2, NULL)) == -1) {
      perror("kevent");
      _exit(EXIT_FAILURE);
    }

    while (nevs --> 0) {
      p = evs[nevs].udata;
      switch (p->type) {
      case ASYNC_TYPE_READ:
        if ((res = read(p->payload.read_data.fd, p->payload.read_data.buf, p->payload.read_data.len)) == -1) {
          perror("read");
          _exit(EXIT_FAILURE);
        }
        p->payload.read_data.f(res, p->payload.read_data.p);
        break;
      }
    }
  }
}

void
async__init_kqueue(async__vtable_t *p)
{
  if ((async__kqueuefd = kqueue()) == -1) {
    perror("kqueue");
    _exit(EXIT_FAILURE);
  }

  p->read = async__read_kqueue;
  p->loop = async__loop_kqueue;
}
