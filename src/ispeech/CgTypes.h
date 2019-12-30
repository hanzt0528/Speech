/////////////////////////////////////////////////////////////////////////
// FileName: def.h
// Description: all type define of icg
// Date: 2019.03
// Author: Zhongtao Han<hanzhongtao@qiyi.com>
/////////////////////////////////////////////////////////////////////////

#ifndef __ICG_
#define __ICG_

 
#include "stdint.h"
#include "stdlib.h"
#include "limits.h"

namespace vfx_icg
{


  enum CgDrawType
  {
   cg_drawtype_none  = -1,
   cg_drawtype_text  = 0,
   cg_drawtype_vertex =1
  };
  
  enum CgPropertyType
  {
   cg_property_type_surface = 1,//表面
   cg_property_type_inner   = 2,//内边
   cg_property_type_outer   = 4,//外边
   cg_property_type_side    = 8,//侧边
   cg_property_type_all     = 15//以上所有
  
  };

  //https://www.freetype.org/freetype2/docs/reference/ft2-font_formats.html#ft_get_font_format
  enum CgFontFormat
  {
   cg_font_format_unknow = 0,
   cg_font_format_truetype,//TrueType
   cg_font_format_type1,//Type 1
   cg_font_format_bdf,//BDF
   cg_font_format_pcf,//PCF
   cg_font_format_type42,//Type 42
   cg_font_format_cid_type1,//CID Type 1
   cg_font_format_cff,//CFF
   cg_font_format_pfr,//PFR
   cg_font_format_windows_fnt//Windows FNT
  };

  struct CgPropertyParam
  {
    uint32_t font_size;//in pixel
    uint32_t* inner_size;//in pixel
    uint32_t inner_style;
    uint32_t* outer_size;//in pixel
    uint32_t outer_style;
    uint32_t* side_size;//in pixel
    uint32_t side_angle;
    bool antialiasing;//do you apply antialiase
    bool surface;//do you fill surface?
    bool horizontal;
  };

  struct CgMetrics
  {
    uint32_t x_ppem;
    uint32_t y_ppem;
    uint32_t horiAdvance;
    uint32_t vertAdvance;
    int32_t  xMin;
    int32_t  xMax;
    int32_t  yMin;
    int32_t  yMax;
  };

  struct  CgBitmap
  {
    uint8_t *buffer;
    uint64_t pitch;
    uint64_t width;
    uint64_t height;
    CgMetrics metrics;
  };

  enum CgFontEncoding
  {
   cg_encoding_none = 0,            
   cg_encoding_unicode,         
   cg_encoding_symbol,          
   cg_encoding_latin_1,        
   cg_encoding_latin_2,     
   cg_encoding_sjis,           
   cg_encoding_gb2312,          
   cg_encoding_big5,           
   cg_encoding_wansung,         
   cg_encoding_johab,           
   cg_encoding_adobe_standard,  
   cg_encoding_adobe_expert,    
   cg_encoding_adobe_custom,    
   cg_encoding_apple_roman     
  };
    
}

#endif
