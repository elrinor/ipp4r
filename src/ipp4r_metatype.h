#ifndef __IPP4R_METATYPE_H__
#define __IPP4R_METATYPE_H__

#include "ipp4r_macro.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------- //
// Supported DataTypes & Channels
// -------------------------------------------------------------------------- //
#define SUPPORTED_DATATYPES (3, (8u, 16u, 32f))
#define SUPPORTED_CHANNELS (3, (C1, C3, AC4))


// -------------------------------------------------------------------------- //
// METATYPE definition
// -------------------------------------------------------------------------- //
#define SUPPORTED_METATYPES (9, (8u_C1, 8u_C3, 8u_AC4, 16u_C1, 16u_C3, 16u_AC4, 32f_C1, 32f_C3, 32f_AC4))
#define METATYPE_DATATYPES  (9, (8u,    8u,    8u,     16u,    16u,    16u,     32f,    32f,    32f))
#define METATYPE_CHANNELS   (9, (   C1,    C3,    AC4,     C1,     C3,     AC4,     C1,     C3,     AC4))


// -------------------------------------------------------------------------- //
// Compile-time map from METATYPE to index in SUPPORTED_METATYPES array
// -------------------------------------------------------------------------- //
#define M_INDEX(METATYPE) M_INDEX_I(METATYPE)
#define M_INDEX_I(METATYPE) M_INDEX_II(M_INDEX_ ## METATYPE ())
#define M_INDEX_II(res) res

#define M_INDEX_8u_C1()    0
#define M_INDEX_8u_C3()   1
#define M_INDEX_8u_AC4()  2
#define M_INDEX_16u_C1()  3
#define M_INDEX_16u_C3()  4
#define M_INDEX_16u_AC4() 5
#define M_INDEX_32f_C1()  6
#define M_INDEX_32f_C3()  7
#define M_INDEX_32f_AC4() 8


// -------------------------------------------------------------------------- //
// Compile-time METATYPE accessors
// -------------------------------------------------------------------------- //
#define M_DATATYPE(METATYPE) ARX_ARRAY_ELEM(M_INDEX(METATYPE), METATYPE_DATATYPES)
#define M_CHANNELS(METATYPE) ARX_ARRAY_ELEM(M_INDEX(METATYPE), METATYPE_CHANNELS)


// -------------------------------------------------------------------------- //
// METATYPE support macros
// -------------------------------------------------------------------------- //


// -------------------------------------------------------------------------- //
// IppMetaType enum
// -------------------------------------------------------------------------- //
typedef enum {
#define ENUM_METATYPE_I(METATYPE, ARG) ARX_JOIN(ipp, METATYPE),
  ARX_ARRAY_FOREACH(SUPPORTED_METATYPES, ENUM_METATYPE_I, ARX_EMPTY)
#undef ENUM_METATYPE_I
} IppMetaType;


// -------------------------------------------------------------------------- //
// METACALL
// -------------------------------------------------------------------------- //
#define METACALL(CRITERIA, TARGET, CRITERIA_ARRAY, FUNCTION, ARGS, DEFAULTVALUE) \
  switch(CRITERIA) {                                                            \
  ARX_ARRAY_FOREACH(CRITERIA_ARRAY, IPPMETACALL_I, (3, (TARGET, FUNCTION, ARGS))) \
  default:                                                                      \
    TARGET = DEFAULTVALUE;                                                      \
  }

#define METACALL_I(METATYPE, ARGS)                                              \
  case ARX_JOIN(ipp, METATYPE):                                                 \
    ARX_ARRAY_ELEM(0, ARGS) = ARX_ARRAY_ELEM(1, ARGS)(METATYPE, ARX_ARRAY_ELEM(2, ARGS)); \
    break;


// -------------------------------------------------------------------------- //
// Standard metafunction for METACALL
// -------------------------------------------------------------------------- //
#define METAFUNC(METATYPE, ARGS)                                                \
  ARX_JOIN(ARX_TUPLE_ELEM(3, 0, ARGS), ARX_JOIN(METATYPE, ARX_TUPLE_ELEM(3, 1, ARGS))) ARX_TUPLE_ELEM(3, 2, ARGS)


#ifdef __cplusplus
}
#endif

#endif