#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
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

int async__epollfd;

void
async__read_epoll(int fd, void *buf, size_t len, void (*f)(int, void *), void *p)
{
  async__event_data_t *data;
  struct epoll_event ev;

  data = malloc(sizeof(async__event_data_t));
  data->type = ASYNC_TYPE_READ;
  data->payload.read_data.fd = fd;
  data->payload.read_data.buf = buf;
  data->payload.read_data.len = len;
  data->payload.read_data.f = f;
  data->payload.read_data.p = p;

  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.ptr = data;

  if (epoll_ctl(async__epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    perror("epoll_ctl");
    _exit(EXIT_FAILURE);
  }
}

void
async__loop_epoll(void)
{
  int nevs, res;
  struct epoll_event evs[10];
  async__event_data_t *p;

  for (;;) {
    if ((nevs = epoll_wait(async__epollfd, evs, 10, -1)) == -1) {
      perror("epoll_wait");
      _exit(EXIT_FAILURE);
    }

    while (nevs --> 0) {
      p = evs[nevs].data.ptr;
      switch (p->type) {
      case ASYNC_TYPE_READ:
        if ((res = read(p->payload.read_data.fd, p->payload.read_data.buf, p->payload.read_data.len)) == -1) {
          perror("read");
          _exit(EXIT_FAILURE);
        }
        p->payload.read_data.f(res, p->payload.read_data.p);
        if (epoll_ctl(async__epollfd, EPOLL_CTL_DEL, p->payload.read_data.fd, NULL) == -1) {
          perror("epoll_ctl");
          _exit(EXIT_FAILURE);
        }
        break;
      }
    }
  }
}

void
async__init_epoll(async__vtable_t *p)
{
  if ((async__epollfd = epoll_create1(0)) == -1) {
    perror("epoll_create1");
    _exit(EXIT_FAILURE);
  }

  p->read = async__read_epoll;
  p->loop = async__loop_epoll;
}
