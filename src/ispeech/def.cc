#include "def.h"
#include "stdio.h"
#include <thread>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace vfx_icg
{
  ICG_GraphicParameter GPL;
  ICG_RectBox::ICG_RectBox()
  {
    top = 0.0;
    left = 0.0;
    bottom = 0.0;
    right = 0.0;
  }

  ICG_RectBox::~ICG_RectBox()
  {
  }

  ICG_Path::ICG_Path()
  {
    pPrev = NULL;
    x[0] = x[1] = x[2] = 0.0;
    y[0] = y[1] = y[2] = 0.0;
    z[0] = z[1] = z[2] = 0.0;
    t[0] = t[1] = t[2] = 0.0;
    POI = NULL;
    bEdgeType = 0;
    bPointType = 0;
    OutLine = 0;
    pNext = NULL;
  }

  ICG_Path::~ICG_Path()
  {
  }

  ICG_Intersection::ICG_Intersection()
  {
    n = 0;
    for (int i=0; i<MAX_BEZIER_INTERSECTION_POINT; i++)
      {
	x[i] = 0.0;
	y[i] = 0.0;
	t[i] = 0.0;
      }
  }

  ICG_Intersection::~ICG_Intersection()
  {
  }

  ICG_Graphic::ICG_Graphic()
  {
    pPrev = NULL;
    pCLHead = NULL;
    pCLTail = NULL;
    iDirectionMask = 0;

    pNext = NULL;
    dwCurveListNum = 0;
  }

  ICG_Graphic::~ICG_Graphic()
  {
  }

  uint64_t ICG_Contour::i_count = 0;
  ICG_Contour::ICG_Contour()
  {
    x = 0;
    y = 0;
    z = 0;
    iAlpha1 = 0;
    iAlpha2 = 0;
    iAlpha3 = 0;				
    dx = 0;
    dy = 0;	
    bPointType = 0;
    bEdgeType = 0;
    iPointID = -1;
    Outline = 0;
    pNext = NULL;
    i_count++;
  }

  ICG_Contour::~ICG_Contour()
  {
    //fprintf(stderr,"ICG_Contour::~ICG_Contour count = %d\n",i_count--);
    i_count--;
  }

  uint64_t ICG_Outline::i_count = 0;
  ICG_Outline::ICG_Outline()
  {
    pPrev = NULL;
    pContourHead = NULL;
    pContourTail = NULL;
    iDirectionMask = 0;
    Rect.left = 0;
    Rect.right = 0;
    Rect.top = 0;
    Rect.bottom = 0;
    i_count++;
    pNext = NULL;
  }

  ICG_Outline::~ICG_Outline()
  {
    i_count--;
  }

  ICG_TextureProperty::ICG_TextureProperty()
  {
    lpTextureBuf = nullptr;
    iBufWidth = 0;
    iBufHeight = 0;
  }

  ICG_TextureProperty::~ICG_TextureProperty()
  {
    Close();
  }

  bool ICG_TextureProperty::Init(int iWidth, int iHeight)
  {
    if( lpTextureBuf != nullptr)
      {
	free(lpTextureBuf);
	lpTextureBuf = nullptr;
      }
    iBufWidth = 0;
    iBufHeight = 0;

    if (iWidth<=0 || iHeight<=0)
      return true;

    lpTextureBuf = (uint32_t *)calloc(1, iWidth*iHeight*sizeof(uint32_t));
    if (lpTextureBuf == nullptr)
      {
	return false;
      }
    iBufWidth = iWidth;
    iBufHeight = iHeight;

    return true;
  }

  bool ICG_TextureProperty::Close()
  {
    if (lpTextureBuf != nullptr)
      {
	free(lpTextureBuf);
      }
    lpTextureBuf = NULL;
    iBufWidth = 0;
    iBufHeight = 0;

    return true;
  }

  ICG_GradientProperty::ICG_GradientProperty()
  {
    lpColorBuf = nullptr;
    iBufWidth = 0;
    iBufHeight = 0;
  }

  ICG_GradientProperty::~ICG_GradientProperty()
  {
    Close();
  }

  bool ICG_GradientProperty::Init(int iWidth, int iHeight)
  {
    if (lpColorBuf != nullptr)
      {
	free(lpColorBuf);
	lpColorBuf = nullptr;
      }
    iBufWidth = 0;
    iBufHeight = 0;
    if (iWidth<=0 || iHeight<=0)
      return true;
    lpColorBuf = (uint32_t *)calloc(1, iWidth*iHeight*sizeof(uint32_t));
    if (lpColorBuf == nullptr)
      {
	return false;
      }
    iBufWidth = iWidth;
    iBufHeight = iHeight;

    return true;
  }

  bool ICG_GradientProperty::Close()
  {
    if (lpColorBuf != nullptr)
      {
	free(lpColorBuf);
      }
    lpColorBuf = nullptr;
    iBufWidth = 0;
    iBufHeight = 0;

    return true;
  }

  ICG_SurfaceProperty::ICG_SurfaceProperty()
  {
    iType = -1;
    TextureProp.Init(0, 0);
    GradientProp.Init(0, 0);
    bSurfaceEmboss = false;
  }

  ICG_SurfaceProperty::~ICG_SurfaceProperty()
  {
    TextureProp.Close();
    GradientProp.Close();
  }

  ICG_BorderProperty::ICG_BorderProperty()
  {
  }

  ICG_BorderProperty::~ICG_BorderProperty()
  {

  }

  ICG_SideProperty::ICG_SideProperty()
  {
  }

  ICG_SideProperty::~ICG_SideProperty()
  {
  }

  ICG_ShadowProperty::ICG_ShadowProperty()
  {
  }

  ICG_ShadowProperty::~ICG_ShadowProperty()
  {
  }

  ICG_PaintProperty::ICG_PaintProperty()
  {
    bOuterBorderAsFace = false;
  }

  ICG_PaintProperty::~ICG_PaintProperty()
  {
  }


#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#endif

  //log

    static int icg_log_level = ICG_LOG_DEBUG;
    static int icg_log_flags;
   static std::mutex mtx_log;

    static void (*icg_log_callback)(void*, int, const char*, va_list) =
    icg_log_default_callback;

    void icg_log(void* avcl, int level, const char *fmt, ...)
    {

      va_list vl;
      va_start(vl, fmt);
      icg_vlog(avcl, level, fmt, vl);
      va_end(vl);
    }

    void icg_vlog(void* avcl, int level, const char *fmt, va_list vl)
    {
      void (*icg_callback)(void*, int, const char*, va_list) = icg_log_callback;
      if (icg_callback)
        icg_callback(avcl, level, fmt, vl);
    }

    int icg_log_get_level(void)
    {
      return icg_log_level;
    }

    void icg_log_set_level(int level)
    {
      icg_log_level = level;
    }

    void icg_log_set_flags(int arg)
    {
      icg_log_flags = arg;
    }

    void icg_log_set_callback(void (*callback)(void*, int, const char*, va_list))
    {
      icg_log_callback = callback;
    }

    void icg_log_default_callback(void* ptr, int level, const char* fmt, va_list vl)
    {
        std::lock_guard<std::mutex> guard(mtx_log);
        
        if (level > icg_log_level)
            return;

        static char message[4096];

        vsnprintf(message,4096,fmt, vl);

#if defined(ANDROID) || defined(__ANDROID__)
        int android_log_level = ANDROID_LOG_INFO;
        switch(level)
        {
            case ICG_LOG_QUIET:
                android_log_level = ANDROID_LOG_SILENT;break;
            case ICG_LOG_VERBOSE:
                android_log_level = ANDROID_LOG_VERBOSE;break;
            case ICG_LOG_DEBUG:
                android_log_level = ANDROID_LOG_DEBUG;break;
            case ICG_LOG_INFO:
                android_log_level = ANDROID_LOG_INFO;break;
            case ICG_LOG_WARNING:
                android_log_level = ANDROID_LOG_WARN;break;
            case ICG_LOG_FATAL:
                android_log_level = ANDROID_LOG_FATAL;break;
            case  ICG_LOG_ERROR:
                android_log_level = ANDROID_LOG_ERROR;break;

        }
        __android_log_print(android_log_level, "icg", "%s,this = %p", message,ptr);
#else
        fprintf(stderr,"\033[31m%s\n", message);
#endif

    }

}
