#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


#include <ctype.h>
#include <inttypes.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdlib.h>

#define LOG_COL_NONE    ""
#define LOG_COL_RST     "\x1b[0m"
#define LOG_COL_DBG     "\x1b[02m"
#define LOG_COL_EXPR    "\x1b[36m"
#define LOG_COL_VAL     "\x1b[01m"
#define LOG_COL_TYPE    "\x1b[32m"
#define LOG_DBG_H_COL(out, col)          (isatty(fileno(out)) ? (col): LOG_COL_NONE)
#define LOG_xstr0(x) #x
#define LOG_xstr(x)  LOG_xstr0(x)
#define LOG_DBG_H_FILE __BASE_FILE__

#define __log_dbg(out, x, fs)      ({                                          \
    typeof(x) _x0 = (x);                                                            \
    log_debug(\
        "%s %s%s%s = %s" LOG_xstr(fs) "%s (%s%s%s)",       \
                LOG_DBG_H_COL(out, LOG_COL_RST),                                            \
                LOG_DBG_H_COL(out, LOG_COL_EXPR), #x, LOG_DBG_H_COL(out, LOG_COL_RST),              \
                LOG_DBG_H_COL(out, LOG_COL_VAL), _x0, LOG_DBG_H_COL(out, LOG_COL_RST),              \
                LOG_DBG_H_COL(out, LOG_COL_TYPE), #fs, LOG_DBG_H_COL(out, LOG_COL_RST)              \
            );                                                                      \
    _x0;                                                                            \
})

#define __log_dbg_ok(out, x, fs)      ({                                          \
    typeof(x) _x0 = (x);                                                            \
    int _n0 = fprintf(out, "%s[%s:%d (%s)]%s %s%s%s = %s" LOG_xstr(fs) "%s (%s%s%s)\n",       \
                LOG_DBG_H_COL(out, LOG_COL_DBG),                                            \
                basename(LOG_DBG_H_FILE), __LINE__, __func__,                 \
                LOG_DBG_H_COL(out, LOG_COL_RST),                                            \
                LOG_DBG_H_COL(out, LOG_COL_EXPR), #x, LOG_DBG_H_COL(out, LOG_COL_RST),              \
                LOG_DBG_H_COL(out, LOG_COL_VAL), _x0, LOG_DBG_H_COL(out, LOG_COL_RST),              \
                LOG_DBG_H_COL(out, LOG_COL_TYPE), #fs, LOG_DBG_H_COL(out, LOG_COL_RST)              \
            );                                                                      \
    assert(_n0 > 0);                                                                \
    _x0;                                                                            \
})


#ifndef LOGLEVEL
#define LOGLEVEL 4
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOGLEVEL < 3
#define NDEBUG 1
#endif

#ifdef NDEBUG
#define log_debug(M, ...)
#else
#ifdef LOG_NOCOLORS
  #define log_debug(M, ...) fprintf(stderr, "DEBUG " M " at %s (%s:%d) \n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
#else
  #define log_debug(M, ...) fprintf(stderr, "\33[34mDEBUG\33[39m " M "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
#endif /* NOCOLORS */
#endif /* NDEBUG */

/* safe readable version of errno */
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef LOG_NOCOLORS
  #define log_error(M, ...) fprintf(stderr,  "ERR   " M " at %s (%s:%d) errno:%s\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
  #define log_warn(M, ...) fprintf(stderr, "WARN  " M " at %s (%s:%d) errno:%s\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
  #define log_info(M, ...) fprintf(stderr, "INFO  " M " at %s (%s:%d)\n", ##__VA_ARGS__, __func__, __FILENAME__, __LINE__)
#else
  #define log_error(M, ...) fprintf(stderr,  "\33[31mERR\33[39m   " M "  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
  #define log_warn(M, ...) fprintf(stderr, "\33[91mWARN\33[39m  " M "  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())
  #define log_info(M, ...) fprintf(stderr, "\33[32mINFO\33[39m  " M "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILENAME__, __LINE__)
#endif /* NOCOLORS */

#if LOGLEVEL < 4
#undef log_info
#define log_info(M, ...)
#endif

#if LOGLEVEL < 2
#undef log_warn
#define log_warn(M, ...)
#endif

#if LOGLEVEL < 1
#undef log_error
#define log_error(M, ...)
#endif

#define log_dbg(x, fs) __log_dbg(stdout, x, fs)
#define log_dbe(x, fs)  __log_dbg(stderr, x, fs)

#define Dbg log_dbg
#define Dbge log_dbge
#define Info log_info
#define Debug log_debug
#define Error log_error
#define Warn log_warn

#endif
