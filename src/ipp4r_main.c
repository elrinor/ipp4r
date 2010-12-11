#include <assert.h>
#include <ruby.h>
#include <ipp.h>

#define IPP4R_MAIN
#include "ipp4r.h"

/* THE GLOBAL TODO
 * + Ipp::ColorRef
 * + Ipp::Image#[x, y]
 * + Ipp::Image#[x, y]=
 * + Ipp::Image#sub_image
 * - Ipp::Image#threshold!
 * - freeze issue
 * - taint issue
 */

/* 2ASK:
 * - portability - do we need it, and what platforms / compilers we need to support
 * - unit tests
 * - other ipp functionality
 */

// TODO: other compilers...
#if defined(_MSC_VER) || defined(__ICL)
#  pragma comment(lib, "ippi.lib")
#  pragma comment(lib, "ippcc.lib")
#  pragma comment(lib, "ippcore.lib")
#  pragma comment(lib, "highgui.lib") /* REMOVE ME */
#  pragma comment(lib, "cxcore.lib") /* REMOVE ME */
#endif


// -------------------------------------------------------------------------- //
// Local accesors
// -------------------------------------------------------------------------- //
DEFINE_ACCESSOR(Color, r, INT)
DEFINE_ACCESSOR(Color, g, INT)
DEFINE_ACCESSOR(Color, b, INT)
DEFINE_ACCESSOR(Color, a, INT)

DEFINE_READER(ColorRef, x, INT)
DEFINE_READER(ColorRef, y, INT)


// -------------------------------------------------------------------------- //
// Init
// -------------------------------------------------------------------------- //
/**
 * Ruby Init routine.
 */
void Init_ipp4r() {
  /* Init IDs first */
#define IPP4R_ID_SET_I(NAMES, ARG)                                             \
  ARX_JOIN(rb_ID_, ARX_TUPLE_ELEM(2, 0, NAMES)) = rb_intern((ARX_TUPLE_ELEM(2, 1, NAMES)) ? (ARX_TUPLE_ELEM(2, 1, NAMES)) : (ARX_STRINGIZE(ARX_TUPLE_ELEM(2, 0, NAMES))));
  ARX_ARRAY_FOREACH(IPP4R_IDS, IPP4R_ID_SET_I, ARX_EMPTY)
#undef IPP4R_ID_SET_I

  /* Then init Ipp module */
  rb_Ipp = rb_define_module("Ipp");

  /* Then enums */
  rb_Enum = rb_define_class_under(rb_Ipp, "Enum", rb_cObject);
  rb_include_module(rb_Enum, rb_mComparable);
  rb_define_alloc_func(rb_Enum, rb_Enum_alloc);
  rb_define_method(rb_Enum, "initialize", rb_Enum_initialize, 2);
  rb_define_method(rb_Enum, "to_s", rb_Enum_to_s, 0);
  rb_define_method(rb_Enum, "to_i", rb_Enum_to_i, 0);
  rb_define_method(rb_Enum, "<=>", rb_Enum_spaceship, 1);
  rb_define_method(rb_Enum, "===", rb_Enum_case_eq, 1);

  ENUM_DEF(rb_Channels, "Channels")
    ENUM(ippC1,  "IppC1")
    ENUM(ippC3,  "IppC3")
    ENUM(ippAC4, "IppAC4")
  ENUM_END()

  ENUM_DEF(rb_DataType, "DataType")
    ENUM(ipp8u,  "Ipp8u")
    ENUM(ipp16u, "Ipp16u")
    ENUM(ipp32f, "Ipp32f")
  ENUM_END()

  ENUM_DEF(rb_CmpOp, "CmpOp")
    ENUM(ippCmpLess,    "LessThan")
    ENUM(ippCmpGreater, "GreaterThan")
  ENUM_END()

  /* And all other classes */
  rb_Image = rb_define_class_under(rb_Ipp, "Image", rb_cObject);
  rb_define_alloc_func(rb_Image, rb_Image_alloc);
  rb_define_method(rb_Image, "initialize", rb_Image_initialize, -1);
  rb_define_method(rb_Image, "initialize_copy", rb_Image_initialize_copy, 1);
  rb_define_method(rb_Image, "save", rb_Image_save, 1);
  rb_define_method(rb_Image, "add_rand_uniform!", rb_Image_add_rand_uniform_bang, 2);
  rb_define_method(rb_Image, "add_rand_uniform", rb_Image_add_rand_uniform, 2);
  rb_define_method(rb_Image, "convert!", rb_Image_convert_bang, 1);
  rb_define_method(rb_Image, "convert", rb_Image_convert, 1);
  rb_define_method(rb_Image, "width", rb_Image_width, 0);
  rb_define_method(rb_Image, "height", rb_Image_height, 0);
  rb_define_method(rb_Image, "channels", rb_Image_channels, 0);
  rb_define_method(rb_Image, "[]", rb_Image_ref, 2);
  rb_define_method(rb_Image, "[]=", rb_Image_ref_eq, 3);
  rb_define_method(rb_Image, "fill!", rb_Image_fill_bang, 1);
  rb_define_method(rb_Image, "fill", rb_Image_fill, 1);
  rb_define_method(rb_Image, "transpose", rb_Image_transpose, 0);
  rb_define_method(rb_Image, "subimage", rb_Image_subimage, -1);
  rb_define_method(rb_Image, "threshold", rb_Image_threshold, -1);

  rb_Data = rb_define_class_under(rb_Image, "Data", rb_cObject);

  rb_Color = rb_define_class_under(rb_Ipp, "Color", rb_cObject);
  rb_define_alloc_func(rb_Color, rb_Color_alloc);
  rb_define_method(rb_Color, "initialize", rb_Color_initialize, -1);
  rb_define_method(rb_Color, "to_s", rb_Color_to_s, 0);
  RB_DEFINE_ACCESSOR(Color, r);
  RB_DEFINE_ACCESSOR(Color, g);
  RB_DEFINE_ACCESSOR(Color, b);
  RB_DEFINE_ACCESSOR(Color, a);
  rb_define_method(rb_Color, "gray", rb_Color_gray, 0);

  rb_ColorRef = rb_define_class_under(rb_Image, "ColorRef", rb_cObject);
  rb_define_method(rb_ColorRef, "to_s", rb_Color_to_s, 0);
  RB_DEFINE_ACCESSOR(ColorRef, r);
  RB_DEFINE_ACCESSOR(ColorRef, g);
  RB_DEFINE_ACCESSOR(ColorRef, b);
  RB_DEFINE_ACCESSOR(ColorRef, a);
  RB_DEFINE_READER(ColorRef, x);
  RB_DEFINE_READER(ColorRef, x);
  rb_define_method(rb_ColorRef, "gray", rb_Color_gray, 0);

  rb_Exception = rb_define_class_under(rb_Ipp, "Exception", rb_eStandardError);

  // forbid new()
  rb_funcall(rb_Data, rb_ID_private_class_method, 1, ID2SYM(rb_ID_new));
  rb_funcall(rb_ColorRef, rb_ID_private_class_method, 1, ID2SYM(rb_ID_new));
}


