/////////////////////////////////////////////////////////////////////////
// FileName: def.h
// Description: some type define of icg
// Date: 2019.03
// Author: Zhongtao Han<hanzhongtao@qiyi.com>
/////////////////////////////////////////////////////////////////////////

#ifndef __DEF_
#define __DEF_

 
#include "stdint.h"
#include "stdlib.h"
#include "limits.h"
#include <cstdarg>
#include <cstdio>

namespace vfx_icg
{
#define icg_version "v0.9.5"
#define __max(a,b)    (((a) > (b)) ? (a) : (b))
#define __min(a,b)    (((a) < (b)) ? (a) : (b))
#define ICG_Precision 0.001465	//1.5/1024
#define ScaledGray 256
#define PixelScale 9
#define ScaledPixel 512
#define ScaledPixel_Half 256
#define ScaledPixel_Sqrt2 724
#define ScaledPixel_2 1024
#define ScaledPixel_4 2048
#define PI   3.14159265
#define MAX_BEZIER_INTERSECTION_POINT 5
#define MAX_POINT_CORD_NUM 512
#define BorderNum_Max 10
#define SIGN(a)  (((a)==0)?0:(((a)>0)?1:-1))
#define HRESULT long
#define NS_NOERROR 0
#define NS_E_FAILED -1


  //TrueType Define
  /*
    A glyph outline is returned as a series of one or more contours defined by a TTPOLYGONHEADER structure followed by one or more curves. 
    Each curve in the contour is defined by a TTPOLYCURVE structure followed by a number of POINTFX data points. 
    POINTFX points are absolute positions, not relative moves. 
    The starting point of a contour is given by the pfxStart member of the TTPOLYGONHEADER structure. 
    The starting point of each curve is the last point of the previous curve or the starting point of the contour. 
    The count of data points in a curve is stored in the cpfx member of TTPOLYCURVE structure. 
    The size of each contour in the buffer, in bytes, is stored in the cb member of TTPOLYGONHEADER structure. 
    Additional curve definitions are packed into the buffer following preceding curves and additional contours are packed into the buffer following preceding contours. 

  */
#ifndef TT_POLYGON_TYPE
#define TT_POLYGON_TYPE 24

#define TT_PRIM_LINE       1
#define TT_PRIM_QSPLINE    2
#define TT_PRIM_CSPLINE    3

  typedef float FLT;
  typedef struct _FIXED {
#ifndef _MAC
    uint16_t  fract;
    int16_t   value;
#else  
    int16_t   value;
    uint16_t  fract;
#endif
  } FIXED,*LPFIXED;

  typedef struct tagPOINTFX
  {
    FIXED x;
    FIXED y;
  } POINTFX, * LPPOINTFX;

  //The TTPOLYCURVE structure contains information about a curve in the outline of a TrueType character.
  typedef struct tagPOLYCURVE
  {
    //The type of curve described by the structure. This member can be one of the following values.
    //TT_PRIM_LINE	Curve is a polyline.
    //TT_PRIM_QSPLINE	Curve is a quadratic Bézier spline.
    //TT_PRIM_CSPLINE	Curve is a cubic Bézier spline.
    uint16_t    wType;

    //The number of POINTFX structures in the array.
    uint16_t    cpfx;

    //Specifies an array of POINTFX structures that define the polyline or Bézier spline.
    FIXED apfx[1];
  } TTPOLYCURVE, * LPTTPOLYCURVE;


  //The TTPOLYGONHEADER structure specifies the starting position and type of a contour in a TrueType character outline.
  //Each TTPOLYGONHEADER structure is followed by one or more TTPOLYCURVE structures.
  typedef struct tagTTPOLYGONHEADE
  {
    uint32_t    cb;//The number of bytes required by the TTPOLYGONHEADER structure and TTPOLYCURVE structure or structures required to describe the contour of the character.
    uint32_t    dwType;//The type of character outline returned. Currently, this value must be TT_POLYGON_TYPE.
    POINTFX pfxStart;//he starting point of the contour in the character outline.
  } TTPOLYGONHEADER, * LPTTPOLYGONHEADER;
  //end TrueType

  typedef struct tagRECT
  {
    long left;
    long top;
    long right;
    long bottom;
  }RECT;
#endif

  struct ICG_RectBox
  {
    FLT top;
    FLT left;
    FLT bottom;
    FLT right;

    ICG_RectBox();
    ~ICG_RectBox();
  };

  struct ICG_Path
  {
    ICG_Path* pPrev; 
    FLT x[3]; //pixel coordinate
    FLT y[3];
    FLT z[3]; 
    FLT t[3];
    ICG_Path* POI;
    uint8_t bEdgeType;
    uint8_t bPointType; 				 
    short OutLine;
    ICG_RectBox Rect;
    ICG_Path* pNext; 

    ICG_Path();
    ~ICG_Path();
  };

  struct ICG_Intersection
  {
    int n; 
    FLT x[MAX_BEZIER_INTERSECTION_POINT];  
    FLT y[MAX_BEZIER_INTERSECTION_POINT];  
    FLT t[MAX_BEZIER_INTERSECTION_POINT]; 

    ICG_Intersection();
    ~ICG_Intersection();
  };

  struct ICG_Graphic
  {
    ICG_Graphic* pPrev;
    ICG_Path* pCLHead; 
    ICG_Path* pCLTail;	
    int iDirectionMask;
    ICG_RectBox Rect; 
    ICG_Graphic* pNext;
    unsigned short dwCurveListNum;

    ICG_Graphic();
    ~ICG_Graphic();
  };

  //该结构体成员顺序和内容不能轻易改动
  struct ICG_Contour
  {
    ICG_Contour *pPrev; 
    int x; 
    int y; 
    int z; 
    int iAlpha1;
    int iAlpha2;
    int iAlpha3;				
    int dx;
    int dy;	
    uint8_t bPointType; 
    uint8_t bEdgeType;
    int  iPointID;
    short Outline; 
    ICG_Contour* pNext; 
    static uint64_t i_count;
    ICG_Contour();
    ~ICG_Contour();
  };
  
  struct ICG_Outline
  {
    ICG_Outline* pPrev; 
    ICG_Contour* pContourHead;
    ICG_Contour* pContourTail;
    int iDirectionMask;
    RECT Rect;
    ICG_Outline* pNext; 
    static uint64_t i_count;
    
    ICG_Outline();
    ~ICG_Outline();
  };

  struct ICG_OutlineGroup
  {
    ICG_Outline* pOutlineInner;
    ICG_Outline* pOutlineOuter;
  };

  enum ICG_MaskType
  {
   asSpecial = -3,
   asSide = -2,
   asOuter = -1,
   asUnknown = 0,
   asEdge = 1,
   asInner = 2
  };

  struct ICG_IMASK
  {
    int r;		 
    int r_Side; 
    int iArea;								
    short iType;
    bool bAntialiasing;
  };

  struct ICG_PIXEL_ElEMENT
  {
    int width;
    int height;
    int zoom;//box边界放大的像素数
    ICG_IMASK *mask;
  };

  struct ICG_hGlobalList
  {
    void* hMem;
    ICG_hGlobalList* pNexthGlobal;
  };

  struct ICG_GraphicParameter
  {
    int Atn[1025];
    int Sin[16385];
    int Cos[16385];
    int X_1[1025];
    int Tran[256];
    int XX[20];
    int YY[20];
    int MyScale;
    int TWOPI;
    int PIONE;
    int PITWO;
    int PIFOUR;
    int* PSQRT;
  };

  struct  ICG_TextureProperty
  {
    uint32_t * lpTextureBuf;	
    int iBufWidth;			
    int iBufHeight;		        

    ICG_TextureProperty();
    ~ICG_TextureProperty();
    bool Init(int iWidth, int iHeight);
    bool Close();
  };

  struct  ICG_GradientProperty
  {
    uint32_t * lpColorBuf;	
    int iBufWidth;		
    int iBufHeight;		

    ICG_GradientProperty();
    ~ICG_GradientProperty();
    bool Init(int iWidth, int iHeight);
    bool Close();
  };

  struct  ICG_SurfaceEmboss
  {
    short iLight_XY;
    short iLight_XZ;
    short iEmbossHeight;
    short iSoftness;	
    short iBalance;		
    bool bSRadius;		
  };

  struct  ICG_SurfaceProperty
  {
    int iType;
    ICG_TextureProperty TextureProp;	
    ICG_GradientProperty GradientProp;
    bool bSurfaceEmboss;			
    ICG_SurfaceEmboss SurfaceEmboss;		

    ICG_SurfaceProperty();
    ~ICG_SurfaceProperty();
  };

  struct  ICG_BorderProperty
  {
    short iBorderStyle;	
    int iBorderThick;//if it's positive for Inner and negative for Outer.	
    short iLightMode;	
    int iBorderNum;	        
    int iCurThick[BorderNum_Max];					
    ICG_SurfaceProperty SurfaceProp[BorderNum_Max];	

    ICG_BorderProperty();
    ~ICG_BorderProperty();
  };

  struct  ICG_SideProperty
  {
    ICG_SurfaceProperty SurfaceProp;	
    int iSideThick;// it is negative				
    short iSideAngle_XY;			

    ICG_SideProperty();
    ~ICG_SideProperty();
  };

  struct  ICG_ShadowProperty
  {
    ICG_SurfaceProperty SurfaceProp; 
    short iNature;					
    short iLight_XY;				
    int iOffsetRadius;				
    bool bShadowBlur;				
    double dShadowSoftness;			
    short iShadowStyle;				
    void* lpShadowExt;				

    ICG_ShadowProperty();
    ~ICG_ShadowProperty();
  };

  struct  ICG_PaintProperty
  {
    uint32_t * lpDestBuf;	
    int iDestWidth;		
    int iDestHeight;	
    int iOriLeft;		
    int iOriTop;		
    bool bOuterBorderAsFace; 
    ICG_SurfaceProperty SurfaceProp;
    ICG_BorderProperty InBorderProp;
    ICG_BorderProperty OutBorderProp;
    ICG_SideProperty SideProp;
    ICG_ShadowProperty ShadowProperty;

    ICG_PaintProperty();
    ~ICG_PaintProperty();
  };


    //log


/**
 * Print no output.
 */
#define ICG_LOG_QUIET    -8

/**
 * Something went really wrong and we will crash now.
 */
#define ICG_LOG_PANIC     0

/**
 * Something went wrong and recovery is not possible.
 * For example, no header was found for a format which depends
 * on headers or an illegal combination of parameters is used.
 */
#define ICG_LOG_FATAL     8

/**
 * Something went wrong and cannot losslessly be recovered.
 * However, not all future data is affected.
 */
#define ICG_LOG_ERROR    16

/**
 * Something somehow does not look correct. This may or may not
 * lead to problems. An example would be the use of '-vstrict -2'.
 */
#define ICG_LOG_WARNING  24

/**
 * Standard information.
 */
#define ICG_LOG_INFO     32

/**
 * Detailed information.
 */
#define ICG_LOG_VERBOSE  40

/**
 * Stuff which is only useful for  developers.
 */
#define ICG_LOG_DEBUG    48

/**
 * Extremely verbose debugging, useful for  development.
 */
#define ICG_LOG_TRACE    56


#define ICG_LOG_MAX_OFFSET (ICG_LOG_TRACE - ICG_LOG_QUIET)

    void icg_log(void *avcl, int level, const char *fmt, ...);

    /**
     * @param callback log function, void* is be nullptr or any other , int often is level of log.
     */
    void icg_log_set_callback(void (*callback)(void*, int, const char*, va_list));

    void icg_log_set_level(int level);
    int icg_log_get_level(void);
    void icg_log_set_flags(int arg);

    void icg_log_default_callback(void *avcl, int level, const char *fmt,va_list vl);


    void icg_vlog(void *avcl, int level, const char *fmt, va_list vl);

    //extern ICG_GraphicParameter GPL;
}// end vfx_icg

#endif
