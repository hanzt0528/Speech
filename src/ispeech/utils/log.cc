#include "log.h"
#include "stdio.h"
#include <thread>
#include <sstream>
#include <chrono>
#include <iomanip>

#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#endif

namespace vfx_ispeech
{
  static int ispeech_log_level = ISPEECH_LOG_DEBUG;
  static int ispeech_log_flags;
  static std::mutex mtx_log;

  static void (*ispeech_log_callback)(void*, int, const char*, va_list) =
    ispeech_log_default_callback;

  void ispeech_log(void* avcl, int level, const char *fmt, ...){
    va_list vl;
    va_start(vl, fmt);
    ispeech_vlog(avcl, level, fmt, vl);
    va_end(vl);
  }

  void ispeech_vlog(void* avcl, int level, const char *fmt, va_list vl){
    void (*ispeech_callback)(void*, int, const char*, va_list) = ispeech_log_callback;
    if (ispeech_callback)
      ispeech_callback(avcl, level, fmt, vl);
  }

  int ispeech_log_get_level(void){
    return ispeech_log_level;
  }

  void ispeech_log_set_level(int level){
    ispeech_log_level = level;
  }

  void ispeech_log_set_flags(int arg){
    ispeech_log_flags = arg;
  }

  void ispeech_log_set_callback(void (*callback)(void*, int, const char*, va_list)){
    ispeech_log_callback = callback;
  }

  void ispeech_log_default_callback(void* ptr, int level, const char* fmt, va_list vl){
    std::lock_guard<std::mutex> guard(mtx_log);
        
    if (level > ispeech_log_level)
      return;

    static char message[4096];

    vsnprintf(message,4096,fmt, vl);

#if defined(ANDROID) || defined(__ANDROID__)
    int android_log_level = ANDROID_LOG_INFO;
    switch(level)
      {
      case ISPEECH_LOG_QUIET:
	android_log_level = ANDROID_LOG_SILENT;break;
      case ISPEECH_LOG_VERBOSE:
	android_log_level = ANDROID_LOG_VERBOSE;break;
      case ISPEECH_LOG_DEBUG:
	android_log_level = ANDROID_LOG_DEBUG;break;
      case ISPEECH_LOG_INFO:
	android_log_level = ANDROID_LOG_INFO;break;
      case ISPEECH_LOG_WARNING:
	android_log_level = ANDROID_LOG_WARN;break;
      case ISPEECH_LOG_FATAL:
	android_log_level = ANDROID_LOG_FATAL;break;
      case  ISPEECH_LOG_ERROR:
	android_log_level = ANDROID_LOG_ERROR;break;

      }
    __android_log_print(android_log_level, "ispeech", "%s,this = %p", message,ptr);
#else
    if(level == ISPEECH_LOG_ERROR)
      fprintf(stderr,"\033[31m%s\n", message);
    else if(level == ISPEECH_LOG_INFO)
      fprintf(stderr,"\033[32m%s\n", message);
    else
      fprintf(stderr,"%s\n", message);

    fprintf(stderr,"\x1b[0m");
         
#endif
  }

}// end vfx_ispeech
