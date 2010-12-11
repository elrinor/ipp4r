#ifndef __IPP4R_H__
#define __IPP4R_H__

#include <ippdefs.h>
#include <arx/Preprocessor.h>
#include "ipp4r_fwd.h"
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
IPP4R_EXTERN VALUE rb_ColorRef;

IPP4R_EXTERN VALUE rb_Exception;

IPP4R_EXTERN VALUE rb_Enum;
IPP4R_EXTERN VALUE rb_Channels;
IPP4R_EXTERN VALUE rb_DataType;
IPP4R_EXTERN VALUE rb_CmpOp;


// -------------------------------------------------------------------------- //
// Frequently used IDs
// -------------------------------------------------------------------------- //
#define IPP4R_IDS                                                               \
  (10, (                                                                        \
    (spaceship,            "<=>"),                                              \
    (enumerators,          NULL),                                               \
    (enumerators_by_value, NULL),                                               \
    (r,                    NULL),                                               \
    (g,                    NULL),                                               \
    (b,                    NULL),                                               \
    (a,                    NULL),                                               \
    (new,                  NULL),                                               \
    (private_class_method, NULL),                                               \
    (clone,                NULL)                                                \
  ))

#define IPP4R_ID_DECL_I(NAMES, ARG)                                             \
  IPP4R_EXTERN ID ARX_JOIN(rb_ID_, ARX_TUPLE_ELEM(2, 0, NAMES));
ARX_ARRAY_FOREACH(IPP4R_IDS, IPP4R_ID_DECL_I, ARX_EMPTY)
#undef IPP4R_ID_DECL_I


// -------------------------------------------------------------------------- //
// Supported Channels & DataTypes
// -------------------------------------------------------------------------- //
#define SUPPORTED_DATATYPES (3, (8u, 16u, 32f))
#define SUPPORTED_CHANNELS (3, (C1, C3, AC4))

// -------------------------------------------------------------------------- //
// MetaType infrastructure
// -------------------------------------------------------------------------- //
#define SUPPORTED_METATYPES (9, (8u_C1, 8u_C3, 8u_AC4, 16u_C1, 16u_C3, 16u_AC4, 32f_C1, 32f_C3, 32f_AC4))

typedef enum {
#define ENUM_METATYPE_I(METATYPE, ARG) ARX_JOIN(ipp, METATYPE),
  ARX_ARRAY_FOREACH(SUPPORTED_METATYPES, ENUM_METATYPE_I, ARX_EMPTY)
#undef ENUM_METATYPE_I
} IppMetaType;

#define M_CHANNELS(METATYPE)

#ifdef __cplusplus
}
#endif

#endif