#ifndef __IPP4R_MACROS_H__
#define __IPP4R_MACROS_H__

#include <ruby.h>
#include "arx/Preprocessor.h"

#define IPPDATATYPES (21, (1u, 8u, 8uc, 8s, 8sc, 16u, 16uc, 16s, 16sc, 32u, 32uc, 32s, 32sc, 32f, 32fc, 64u, 64uc, 64s, 64sc, 64f, 64fc))

#define IPPCHANNELS (12, (C0, C1, C2, C3, C4, P2, P3, P4, AC1, AC4, A0C4, AP4))

#define IPPMETACALL(CRITERIA, TARGET, FUNC_PREFIX, FUNC_SUFFIX, FUNC_ARRAY, PARAMS, DEFAULTVALUE) \
  switch(CRITERIA) {                                                            \
  ARX_ARRAY_FOREACH(FUNC_ARRAY, IPPMETACALL_I, (4, (FUNC_PREFIX, FUNC_SUFFIX, TARGET, PARAMS))) \
  default:                                                                      \
  TARGET = DEFAULTVALUE;                                                        \
  }

#define IPPMETACALL_I(FUNC_MIDDLE, ARGS)                                        \
  case ARX_JOIN(ipp, FUNC_MIDDLE):                                              \
  ARX_ARRAY_ELEM(2, ARGS) = ARX_JOIN(ARX_ARRAY_ELEM(0, ARGS), ARX_JOIN(FUNC_MIDDLE, ARX_ARRAY_ELEM(1, ARGS))) ARX_ARRAY_ELEM(3, ARGS); \
  break;


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

#define DEFINE_READER_A(CLASS, ATTR, FIELD, TYPE)                               \
VALUE CLASS ## _ ## ATTR(VALUE self) {                                          \
  CLASS *ptr;                                                                   \
  Data_Get_Struct(self, CLASS, ptr);                                            \
  return C2R_ ## TYPE (ptr->FIELD);                                             \
}

#define DEFINE_READER(CLASS, ATTR, TYPE)                                        \
  DEFINE_READER_A(CLASS, ATTR, ATTR, TYPE)

#define DEFINE_WRITER_A(CLASS, ATTR, FIELD, TYPE)                               \
VALUE CLASS ## _ ## ATTR ## _eq(VALUE self, VALUE val) {                        \
  CLASS *ptr;                                                                   \
  rm_check_frozen(self);                                                        \
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
  rb_define_method(rb_ ## CLASS, #ATTR, CLASS ## _ ## ATTR, 0);

#define RB_DEFINE_WRITER(CLASS, ATTR)                                           \
  rb_define_method(rb_ ## CLASS, #ATTR "=", CLASS ## _ ## ATTR ## _eq, 1);

#define RB_DEFINE_ACCESSOR(CLASS, ATTR)                                         \
  RB_DEFINE_READER(class, attr)                                                 \
  RB_DEFINE_WRITER(class, attr)

#endif