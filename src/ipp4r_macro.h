#ifndef __IPP4r_MACRO_H__
#define __IPP4r_MACRO_H__


// -------------------------------------------------------------------------- //
// IPP Error & Warning macros
// -------------------------------------------------------------------------- //
#ifdef IS_ERROR
#  undef IS_ERROR
#endif
#define IS_ERROR(x) ((x) < 0)

#ifdef IS_WARNING
#  undef IS_WARNING
#endif
#define IS_WARNING(x) ((x) > 0)


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
#define C2R_BOOL(V) ((V) ? Qtrue : Qfalse)
#define R2C_BOOL(V) RTEST(V)
#define C2R_INT(V) INT2FIX(V)
#define R2C_INT(V) NUM2INT(V)
#define C2R_UINT(V) UINT2NUM(V)
#define R2C_UINT(V) NUM2UINT(V)
#define C2R_LONG(V) LONG2FIX(V)
#define R2C_LONG(V) NUM2LONG(V)
#define C2R_STR(V) ((V) ? rb_str_new2(V) : Qnil)
#define R2C_STR(V) RSTRING_PTR(V)
#define C2R_DBL(V) rb_float_new(V)
#define R2C_DBL(V) NUM2DBL(V)
#define R2C_FLT(V) ((float)R2C_DBL(V))
#define C2R_FLT(V) C2R_DBL(V)

#define R2M_NUM(V) C2M_NUMBER_D(32f, R2C_FLT(V))
#define M2R_NUM(V) C2R_FLT(M2C_NUMBER_D(32f, V))

#define C2R_METANUM M2R_NUM
#define R2C_METANUM R2M_NUM

// -------------------------------------------------------------------------- //
// STATIC_ASSERT
// -------------------------------------------------------------------------- //
#define STATIC_ASSERT(expr) {char STATIC_ASSERTION_FAILURE[(expr) ? 1 : 0];}
#define STATIC_ASSERT_D(expr) char ARX_JOIN(STATIC_ASSERTION_FAILURE_, __LINE__)[(expr) ? 1 : 0];


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


// -------------------------------------------------------------------------- //
// Necessary macros
// -------------------------------------------------------------------------- //
#ifdef FALSE
#  undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#  undef TRUE
#endif
#define TRUE 1

#ifdef max
#  undef max
#endif
#define max(a, b) (((a) > (b)) ? (a) : (b))

#ifdef min
#  undef min
#endif
#define min(a, b) (((a) < (b)) ? (a) : (b))

#endif