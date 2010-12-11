#ifndef __IPP4R_H__
#define __IPP4R_H__

#include "ipp4r_image.h"
#include "ipp4r_data.h"
#include "ipp4r_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IPP4R_EXTERN
#  ifdef IPP4R_MAIN
#    define IPP4R_EXTERN
#  else
#    define IPP4R_EXTERN extern
#  endif
#endif


IPP4R_EXTERN VALUE rb_Ipp;

IPP4R_EXTERN VALUE rb_Image;
IPP4R_EXTERN VALUE rb_Data;

IPP4R_EXTERN VALUE rb_Exception;



#ifdef __cplusplus
}
#endif

#endif