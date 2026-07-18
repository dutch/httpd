# HTTPD_HAVE_DEFINE_WITH_EPOLL
# ----------------------------
#
# Check for epoll support on the system.
AC_DEFUN([HTTPD_HAVE_DEFINE_WITH_EPOLL],
  [AC_ARG_WITH([epoll],
     [AS_HELP_STRING([--with-epoll],
        [support epoll in the event backend @<:@default=auto@:>@])],
     [],
     [with_epoll=auto])
   AS_IF([test "x$with_epoll" != xno],
     [AC_CHECK_HEADERS([sys/epoll.h],
        [],
        [if test "x$with_epoll" != xauto; then
             AC_MSG_FAILURE([--with-epoll was given, but test for sys/epoll.h failed])
         fi])])
   AM_CONDITIONAL([HAVE_SYS_EPOLL_H], [test "x$ac_cv_header_sys_epoll_h" = xyes])])


# HTTPD_HAVE_DEFINE_WITH_KQUEUE
# -----------------------------
#
# Check for kqueue support on the system.
AC_DEFUN([HTTPD_HAVE_DEFINE_WITH_KQUEUE],
  [AC_ARG_WITH([kqueue],
     [AS_HELP_STRING([--with-kqueue],
        [support kqueue in the event backend @<:@default=auto@:>@])],
     [],
     [with_kqueue=auto])
   AS_IF([test "x$with_kqueue" != xno],
     [AC_CHECK_HEADERS([sys/event.h],
        [],
        [if test "x$with_kqueue" != xauto; then
             AC_MSG_FAILURE([--with-kqueue was given, but test for sys/event.h failed])
         fi])])
   AM_CONDITIONAL([HAVE_SYS_EVENT_H], [test "x$ac_cv_header_sys_event_h" = xyes])])


# HTTPD_HAVE_DEFINE_WITH_DEPENDENCIES
# -----------------------------------
#
# Check for dependency support on the system.
AC_DEFUN([HTTPD_HAVE_DEFINE_WITH_DEPENDENCIES],
  [HTTPD_HAVE_DEFINE_WITH_EPOLL
   HTTPD_HAVE_DEFINE_WITH_KQUEUE])
