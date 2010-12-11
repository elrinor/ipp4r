#ifndef __IPP4R_H__
#define __IPP4R_H__

#include <ippdefs.h>
#include <arx/Preprocessor.h>
#include "ipp4r_c_image.h"
#include "ipp4r_r_image.h"
#include "ipp4r_data.h"
#include "ipp4r_macros.h"
#include "ipp4r_color.h"
#include "ipp4r_enum.h"
#include "ipp4r_util.h"

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

// -------------------------------------------------------------------------- //
// Global class variables
// -------------------------------------------------------------------------- //
IPP4R_EXTERN VALUE rb_Ipp;

IPP4R_EXTERN VALUE rb_Image;
IPP4R_EXTERN VALUE rb_Data;
IPP4R_EXTERN VALUE rb_Color;

IPP4R_EXTERN VALUE rb_Exception;

IPP4R_EXTERN VALUE rb_Enum;
IPP4R_EXTERN VALUE rb_Channels;


// -------------------------------------------------------------------------- //
// Frequently used IDs
// -------------------------------------------------------------------------- //
#define IPP4R_IDS                                                               \
  (9, (                                                                         \
    (spaceship,            "<=>"),                                              \
    (enumerators,          NULL),                                               \
    (enumerators_by_value, NULL),                                               \
    (red,                  NULL),                                               \
    (green,                NULL),                                               \
    (blue,                 NULL),                                               \
    (new,                  NULL),                                               \
    (private_class_method, NULL),                                               \
    (remove_const,         NULL)                                                \
  ))

#define IPP4R_ID_DECL_I(NAMES, ARG)                                             \
  IPP4R_EXTERN ID ARX_JOIN(rb_ID_, ARX_TUPLE_ELEM(2, 0, NAMES));
ARX_ARRAY_FOREACH(IPP4R_IDS, IPP4R_ID_DECL_I, ARX_EMPTY)
#undef IPP4R_ID_DECL_I

#ifdef __cplusplus
}
#endif

#endif