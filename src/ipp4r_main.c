#include <assert.h>
#include <ruby.h>
#include <ipp.h>

#define IPP4R_MAIN
#include "ipp4r.h"

/* THE GLOBAL TODO
 * - Ipp::ColorRef
 * - Ipp::Image#[x, y]
 * - Ipp::Image#[x, y]=
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
DEFINE_ACCESSOR(Color, red, INT)
DEFINE_ACCESSOR(Color, green, INT)
DEFINE_ACCESSOR(Color, blue, INT)
DEFINE_ACCESSOR(Color, alpha, INT)


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

  /* And all other classes */
  rb_Image = rb_define_class_under(rb_Ipp, "Image", rb_cObject);
  rb_define_alloc_func(rb_Image, rb_Image_alloc);
  rb_define_method(rb_Image, "initialize", rb_Image_initialize, -1);
  rb_define_method(rb_Image, "save", rb_Image_save, 1);
  rb_define_method(rb_Image, "add_rand_uniform!", rb_Image_add_rand_uniform_bang, 2);
  rb_define_method(rb_Image, "add_rand_uniform", rb_Image_add_rand_uniform, 2);
  rb_define_method(rb_Image, "convert!", rb_Image_convert_bang, 1);
  rb_define_method(rb_Image, "convert", rb_Image_convert, 1);
  rb_define_method(rb_Image, "width", rb_Image_width, 0);
  rb_define_method(rb_Image, "height", rb_Image_height, 0);
  rb_define_method(rb_Image, "channels", rb_Image_channels, 0);

  rb_Data = rb_define_class_under(rb_Image, "Data", rb_cObject);

  rb_Color = rb_define_class_under(rb_Ipp, "Color", rb_cObject);
  rb_define_alloc_func(rb_Color, rb_Color_alloc);
  rb_define_method(rb_Color, "initialize", rb_Color_initialize, -1);
  rb_define_method(rb_Color, "to_s", rb_Color_to_s, 0);
  RB_DEFINE_ACCESSOR(Color, red);
  RB_DEFINE_ACCESSOR(Color, green);
  RB_DEFINE_ACCESSOR(Color, blue);
  RB_DEFINE_ACCESSOR(Color, alpha);
  rb_define_method(rb_Color, "gray", rb_Color_gray, 0);

  rb_Exception = rb_define_class_under(rb_Ipp, "Exception", rb_eStandardError);


  // cleanup
  rb_funcall(rb_Ipp,   rb_ID_remove_const, 1, ID2SYM(rb_intern("Enum")));
  rb_funcall(rb_Ipp,   rb_ID_remove_const, 1, ID2SYM(rb_intern("Channels")));
  rb_funcall(rb_Image, rb_ID_remove_const, 1, ID2SYM(rb_intern("Data")));


}


