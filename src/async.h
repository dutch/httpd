#ifndef ASYNC_HEADER
#define ASYNC_HEADER

void async_init(void);
void async_read(int, void *, size_t, void (*)(int, void *), void *);
void async_loop(void);

#endif
