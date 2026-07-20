bin_PROGRAMS = %D%/httpd
%C%_httpd_SOURCES = %D%/httpd.c %D%/async.c

if HAVE_SYS_EPOLL_H
%C%_httpd_SOURCES += %D%/epoll/async.c
endif

if HAVE_SYS_EVENT_H
%C%_httpd_SOURCES += %D%/kqueue/async.c
endif
