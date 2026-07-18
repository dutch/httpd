#include <stdio.h>
#include <unistd.h>
#include "async.h"

typedef struct read_data
{
  char buf[512];
} read_data_t;

void
done_reading(int i, void *p)
{
  printf("got size %d buffer: '%.*s'\n", i, i, ((struct read_data *)p)->buf);
}

int
main(int argc, char **argv)
{
  read_data_t data;

  (void)argc;
  (void)argv;

  async_init();
  async_read(STDIN_FILENO, data.buf, 512, done_reading, &data);
  async_loop();

  return 0;
}
