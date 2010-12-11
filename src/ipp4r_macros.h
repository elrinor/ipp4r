#ifndef __IPP4R_MACROS_H__
#define __IPP4R_MACROS_H__

#include <ruby.h>
#include "arx/Preprocessor.h"

// -------------------------------------------------------------------------- //
// Debug
// -------------------------------------------------------------------------- //
#define DBG() printf("%s", "#####"__FUNCTION__ ":" ARX_STRINGIZE(__LINE__) "!\n");
#define Unreachable() assert(!"Unreachable")

// -------------------------------------------------------------------------- //
// IPP-related macros
// -------------------------------------------------------------------------- //
#define IPPDATATYPES (21, (1u, 8u, 8uc, 8s, 8sc, 16u, 16uc, 16s, 16sc, 32u, 32uc, 32s, 32sc, 32f, 32fc, 64u, 64uc, 64s, 64sc, 64f, 64fc))

#define IPPCHANNELS (12, (C0, C1, C2, C3, C4, P2, P3, P4, AC1, AC4, A0C4, AP4))

#define IPPMETACALL_A(CRITERIA, TARGET, FUNC_PREFIX, FUNC_SUFFIX, FUNC_ARRAY, CRITERIA_ARRAY, PARAMS, DEFAULTVALUE) \
  switch(CRITERIA) {                                                            \
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(FUNC_ARRAY)), IPPMETACALL_I, (6, (FUNC_PREFIX, FUNC_SUFFIX, TARGET, PARAMS, FUNC_ARRAY, CRITERIA_ARRAY))) \
  default:                                                                      \
    TARGET = DEFAULTVALUE;                                                      \
  }

#define IPPMETACALL(CRITERIA, TARGET, FUNC_PREFIX, FUNC_SUFFIX, FUNC_ARRAY, PARAMS, DEFAULTVALUE) \
  IPPMETACALL_A(CRITERIA, TARGET, FUNC_PREFIX, FUNC_SUFFIX, FUNC_ARRAY, FUNC_ARRAY, PARAMS, DEFAULTVALUE)

#define IPPMETACALL_I(INDEX, ARGS)                                              \
  case ARX_JOIN(ipp, ARX_ARRAY_ELEM(INDEX, ARX_ARRAY_ELEM(5, ARGS))):           \
    ARX_ARRAY_ELEM(2, ARGS) = ARX_JOIN(ARX_ARRAY_ELEM(0, ARGS), ARX_JOIN(ARX_ARRAY_ELEM(INDEX, ARX_ARRAY_ELEM(4, ARGS)), ARX_ARRAY_ELEM(1, ARGS))) ARX_ARRAY_ELEM(3, ARGS); \
    break;

#ifdef IS_ERROR
#  undef IS_ERROR
#endif
#define IS_ERROR(x) ((x) < 0)

#ifdef IS_WARNING
#  undef IS_WARNING
#endif
#define IS_WARNING(x) ((x) > 0)

#ifdef FALSE
#  undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#  undef TRUE
#endif
#define TRUE 0


// -------------------------------------------------------------------------- //
// Frozen check
// -------------------------------------------------------------------------- //
#define CHECK_FROZEN(OBJ)                                                       \
  if(OBJ_FROZEN(OBJ)) {                                                         \
    rb_error_frozen(rb_class2name(CLASS_OF(OBJ)));                              \
  }


// -------------------------------------------------------------------------- //
// Type converters
// -------------------------------------------------------------------------- //
#define C2R_BOOL(V) (V) ? Qtrue : Qfalse
#define R2C_BOOL(V) RTEST(V)
#define C2R_INT(V) INT2FIX(V)
#define R2C_INT(V) NUM2INT(V)
#define C2R_UINT(V) UINT2NUM(V)
#define R2C_UINT(V) NUM2UINT(V)
#define C2R_LONG(V) LONG2FIX(V)
#define R2C_LONG(V) NUM2LONG(V)
#define C2R_STR(V) (V) ? rb_str_new2(V) : Qnil
#define R2C_STR(V) RSTRING_PTR(V)
#define C2R_DBL(V) rb_float_new(V)
#define R2C_DBL(V) NUM2DBL(V)


// -------------------------------------------------------------------------- //
// Accessors
// -------------------------------------------------------------------------- //
#define DEFINE_READER_A(CLASS, ATTR, FIELD, TYPE)                               \
VALUE rb_ ## CLASS ## _ ## ATTR(VALUE self) {                                   \
  CLASS *ptr;                                                                   \
  Data_Get_Struct(self, CLASS, ptr);                                            \
  return C2R_ ## TYPE (ptr->FIELD);                                             \
}

#define DEFINE_READER(CLASS, ATTR, TYPE)                                        \
  DEFINE_READER_A(CLASS, ATTR, ATTR, TYPE)

#define DEFINE_WRITER_A(CLASS, ATTR, FIELD, TYPE)                               \
VALUE rb_ ## CLASS ## _ ## ATTR ## _eq(VALUE self, VALUE val) {                 \
  CLASS *ptr;                                                                   \
  CHECK_FROZEN(self);                                                           \
  Data_Get_Struct(self, CLASS, ptr);                                            \
  ptr->FIELD = R2C_ ## TYPE (val);                                              \
  return self;                                                                  \
}

#define DEFINE_WRITER(CLASS, ATTR, TYPE)                                        \
  DEFINE_WRITER_A(CLASS, ATTR, ATTR, TYPE)

#define DEFINE_ACCESSOR_A(CLASS, ATTR, FIELD, TYPE)                             \
  DEFINE_READER_A(CLASS, ATTR, FIELD, TYPE)                                     \
  DEFINE_WRITER_A(CLASS, ATTR, FIELD, TYPE)

#define DEFINE_ACCESSOR(CLASS, ATTR, TYPE)                                      \
  DEFINE_ACCESSOR_A(CLASS, ATTR, ATTR, TYPE)

#define RB_DEFINE_READER(CLASS, ATTR)                                           \
  rb_define_method(rb_ ## CLASS, #ATTR, rb_ ## CLASS ## _ ## ATTR, 0);

#define RB_DEFINE_WRITER(CLASS, ATTR)                                           \
  rb_define_method(rb_ ## CLASS, #ATTR "=", rb_ ## CLASS ## _ ## ATTR ## _eq, 1);

#define RB_DEFINE_ACCESSOR(CLASS, ATTR)                                         \
  RB_DEFINE_READER(CLASS, ATTR)                                                 \
  RB_DEFINE_WRITER(CLASS, ATTR)

#endif