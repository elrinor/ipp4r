#include <ruby.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// color_new
// -------------------------------------------------------------------------- //
Color* color_new(int red, int green, int blue, int alpha) {
  Color* color;
  color = ALLOC(Color); // ALLOC always succeeds or throws an exception
  color->red = red;
  color->green = green;
  color->blue = blue;
  color->alpha = alpha;
  return color;
}

// -------------------------------------------------------------------------- //
// color_destroy
// -------------------------------------------------------------------------- //
void color_destroy(Color* color) {
  xfree(color); // we must use xfree here, since we allocated color with ALLOC
}


// -------------------------------------------------------------------------- //
// rb_Color_alloc
// -------------------------------------------------------------------------- //
VALUE rb_Color_alloc(VALUE klass) {
  VALUE color;
  color = Data_Wrap_Struct(klass, NULL, color_destroy, NULL); /* Underlying C struct will be allocated later, in "initialize" method */
  return color;
}


// -------------------------------------------------------------------------- //
// rb_Color_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Color_initialize(int argc, VALUE *argv, VALUE self) {
  int r, g, b, a;
  Color* color;

  a = 255;

  switch (argc) {
  case 4:
    a = NUM2INT(argv[3]);
  case 3:
    r = NUM2INT(argv[0]);
    g = NUM2INT(argv[1]);
    b = NUM2INT(argv[2]);
    break;
  case 1:
    r = g = b = NUM2INT(argv[0]);
    break;
  case 0:
    r = g = b = 0;
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 0, 1, 3, or 4)", argc);
    break;
  }

  color = color_new(r, g, b, a); // throws, cannot fail

  DATA_PTR(self) = color;

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Color_to_s
// -------------------------------------------------------------------------- //
VALUE rb_Color_to_s(VALUE self) {
  Color* color;
  char buf[100]; // 10 will be enough, but 100 is safer %)

  Data_Get_Struct(self, Color, color);

  sprintf(buf, "#%02x%02x%02x%02x", color->red, color->green, color->blue, color->alpha);
  return rb_str_new2(buf);
}


// -------------------------------------------------------------------------- //
// rb_Color_gray
// -------------------------------------------------------------------------- //
VALUE rb_Color_gray(VALUE self) {
  return C2R_INT((int)(0.299f * R2C_INT(rb_funcall(self, rb_ID_red, 0)) + 0.587f *  R2C_INT(rb_funcall(self, rb_ID_green, 0)) + 0.114f *  R2C_INT(rb_funcall(self, rb_ID_blue, 0))));
}
