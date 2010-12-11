#ifndef __IPP4R_H__
#define __IPP4R_H__

#include <ippdefs.h>
#include "arx/Preprocessor.h"
#include "ipp4r_fwd.h"
#include "ipp4r_c_image.h"
#include "ipp4r_r_image.h"
#include "ipp4r_data.h"
#include "ipp4r_color.h"
#include "ipp4r_enum.h"
#include "ipp4r_util.h"
#include "ipp4r_macro.h"
#include "ipp4r_metatype.h"
#include "ipp4r_struct.h"
#include "ipp4r_matrix.h"

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
// Config
// -------------------------------------------------------------------------- //
/** This one was added for debugging purposes only, you'll never need it (at least I hope so) */
// #define USE_TRACE

/** Use OpenCV for saving / loading? If not defined, only 24-bit bmp files will be supported */
// #define USE_OPENCV


// -------------------------------------------------------------------------- //
// Debug facilities
// -------------------------------------------------------------------------- //
#define Unreachable() assert(!"Unreachable")

#if defined(_MSC_VER) || defined(__ICL)
#  define __func__ __FUNCTION__
#endif

#ifdef USE_TRACE
#  define IS_VOID_HELPER_void 
#  define IS_VOID(v) ARX_IS_EMPTY(ARX_JOIN(IS_VOID_HELPER_, v))
#  define TRACE_HELPER_TAIL() for(trace_i = trace_depth; trace_i > 0; trace_i--) printf("-")
#  define TRACE_HELPER_BORDER(C) TRACE_HELPER_TAIL(); printf(C); printf(__func__); printf("\n")
#  define TRACE_FUNC(RETURN_TYPE, FUNC_NAME, ARGS)                              \
  RETURN_TYPE FUNC_NAME ARGS { int trace_i; ARX_IF(IS_VOID(RETURN_TYPE), ARX_EMPTY(), RETURN_TYPE trace_retval;) TRACE_HELPER_BORDER(">"); trace_depth++;
#  define TRACE_END trace_depth--; TRACE_HELPER_BORDER("<"); }
#  define TRACE_RETURN(val) { trace_retval = (val); trace_depth--; TRACE_HELPER_BORDER("<"); return trace_retval; }
#  define TRACE_RETURN_0() { trace_depth--; TRACE_HELPER_BORDER("<"); return; }
#  define TRACE(PRINTF_ARGS) {TRACE_HELPER_TAIL(); printf(":"); printf PRINTF_ARGS; printf("\n");}
#  define IF_TRACE(EXPR) EXPR
#else
#  define TRACE_FUNC(RETURN_TYPE, FUNC_NAME, ARGS) RETURN_TYPE FUNC_NAME ARGS
#  define TRACE_END
#  define TRACE_RETURN(val) return (val)
#  define TRACE_RETURN_0() return
#  define TRACE(PRINTF_ARGS) 
#  define IF_TRACE(EXPR)
#endif

IF_TRACE(IPP4R_EXTERN int trace_depth;)


// -------------------------------------------------------------------------- //
// Global class variables
// -------------------------------------------------------------------------- //
IPP4R_EXTERN VALUE rb_Ipp;

IPP4R_EXTERN VALUE rb_Image;
IPP4R_EXTERN VALUE rb_Data;
IPP4R_EXTERN VALUE rb_Color;
IPP4R_EXTERN VALUE rb_ColorRef;
IPP4R_EXTERN VALUE rb_Point;
IPP4R_EXTERN VALUE rb_Size;

IPP4R_EXTERN VALUE rb_Exception;

IPP4R_EXTERN VALUE rb_Enum;
IPP4R_EXTERN VALUE rb_Channels;
IPP4R_EXTERN VALUE rb_DataType;
IPP4R_EXTERN VALUE rb_MetaType;
IPP4R_EXTERN VALUE rb_CmpOp;
IPP4R_EXTERN VALUE rb_Axis;
IPP4R_EXTERN VALUE rb_MaskSize;


// -------------------------------------------------------------------------- //
// Frequently used IDs
// -------------------------------------------------------------------------- //
#define IPP4R_IDS                                                               \
  (16, (                                                                        \
    (spaceship,            "<=>"),                                              \
    (aref,                 "[]"),                                               \
    (enumerators,          NULL),                                               \
    (enumerators_by_value, NULL),                                               \
    (r,                    NULL),                                               \
    (g,                    NULL),                                               \
    (b,                    NULL),                                               \
    (a,                    NULL),                                               \
    (new,                  NULL),                                               \
    (private_class_method, NULL),                                               \
    (clone,                NULL),                                               \
    (row_size,             NULL),                                               \
    (column_size,          NULL),                                               \
    (width,                NULL),                                               \
    (height,               NULL),                                               \
    (resize,               NULL)                                                \
  ))

#define IPP4R_ID_DECL_I(NAMES, ARG)                                             \
  IPP4R_EXTERN ID ARX_JOIN(rb_ID_, ARX_TUPLE_ELEM(2, 0, NAMES));
ARX_ARRAY_FOREACH(IPP4R_IDS, IPP4R_ID_DECL_I, ARX_EMPTY)
#undef IPP4R_ID_DECL_I


#ifdef __cplusplus
}
#endif

#endif
