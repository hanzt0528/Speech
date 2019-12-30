#ifndef _ISPEECH_LOG_H
#define _ISPEECH_LOG_H
 
#include "stdint.h"
#include "stdlib.h"
#include "limits.h"
#include <cstdarg>
#include <cstdio>

namespace vfx_ispeech{
  
  /**
   * Print no output.
   */
#define ISPEECH_LOG_QUIET    -8

  /**
   * Something went really wrong and we will crash now.
   */
#define ISPEECH_LOG_PANIC     0

  /**
   * Something went wrong and recovery is not possible.
   * For example, no header was found for a format which depends
   * on headers or an illegal combination of parameters is used.
   */
#define ISPEECH_LOG_FATAL     8

  /**
   * Something went wrong and cannot losslessly be recovered.
   * However, not all future data is affected.
   */
#define ISPEECH_LOG_ERROR    16

  /**
   * Something somehow does not look correct. This may or may not
   * lead to problems. An example would be the use of '-vstrict -2'.
   */
#define ISPEECH_LOG_WARNING  24

  /**
   * Standard information.
   */
#define ISPEECH_LOG_INFO     32

  /**
   * Detailed information.
   */
#define ISPEECH_LOG_VERBOSE  40

  /**
   * Stuff which is only useful for  developers.
   */
#define ISPEECH_LOG_DEBUG    48

  /**
   * Extremely verbose debugging, useful for  development.
   */
#define ISPEECH_LOG_TRACE    56


#define ISPEECH_LOG_MAX_OFFSET (ISPEECH_LOG_TRACE - ISPEECH_LOG_QUIET)

  void ispeech_log(void *avcl, int level, const char *fmt, ...);

  /**
   * @param callback log function, void* is be nullptr or any other , int often is level of log.
   */
  void ispeech_log_set_callback(void (*callback)(void*, int, const char*, va_list));

  void ispeech_log_set_level(int level);
  int ispeech_log_get_level(void);
  void ispeech_log_set_flags(int arg);

  void ispeech_log_default_callback(void *avcl, int level, const char *fmt,va_list vl);

  void ispeech_vlog(void *avcl, int level, const char *fmt, va_list vl);
 
#define LOG_ERR(avcl,format, args...)   ispeech_log(avcl,ISPEECH_LOG_ERROR, format , ##args)
#define LOG_INF(avcl,format, args...)   ispeech_log(avcl,ISPEECH_LOG_INFO, format , ##args)
#define LOG_MSG(avcl,format, args...)   ispeech_log(avcl,ISPEECH_LOG_INFO, format , ##args)
#define LOG_DBG(avcl,format, args...)   ispeech_log(avcl,ISPEECH_LOG_DEBUG, format , ##args)
#define LOG_WRN(avcl,format, args...)   ispeech_log(avcl,ISPEECH_LOG_WARNING, format, ##args)

}// end vfx_ispeech

#endif //end  _ISPEECH_LOG_H
