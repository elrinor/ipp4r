#include <assert.h>
#include <ruby.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// Supplementary finctions
// -------------------------------------------------------------------------- //
/**
 * Gets a color referenced by given ColorRef
 */
static Color* rb_ColorRef_getter(VALUE self, Color* color) {
  ColorRef* colorref;

  rb_ColorRef_check_raise(self);

  colorref = Data_Get_Struct_Ret(self, ColorRef);

  raise_on_error(image_get_pixel(colorref->image, colorref->x, colorref->y, color));

  return color;
}


/**
 * Sets a color referenced by given ColorRef
 */
static void rb_ColorRef_setter(VALUE self, Color* color) {
  ColorRef* colorref;

  /* setter is always called after getter => we don't need to preform boundary checks */
  /* rb_ColorRef_check_raise(self); */

  colorref = Data_Get_Struct_Ret(self, ColorRef);

  raise_on_error(image_set_pixel(colorref->image, colorref->x, colorref->y, color));
}

// -------------------------------------------------------------------------- //
// color_new
// -------------------------------------------------------------------------- //
Color* color_new(int red, int green, int blue, int alpha) {
  Color* color;
  color = ALLOC(Color); // ALLOC always succeeds or throws an exception
  color->r = red;
  color->g = green;
  color->b = blue;
  color->a = alpha;
  return color;
}

// -------------------------------------------------------------------------- //
// color_destroy
// -------------------------------------------------------------------------- //
void color_destroy(Color* color) {
  assert(color != NULL);

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
  char buf[100]; // 10 is enough, but 100 is safer %)

  Data_Get_Struct(self, Color, color);

  sprintf(buf, "#%02x%02x%02x%02x", 
    R2C_INT(rb_funcall(self, rb_ID_r, 0)), R2C_INT(rb_funcall(self, rb_ID_g, 0)), R2C_INT(rb_funcall(self, rb_ID_b, 0)), R2C_INT(rb_funcall(self, rb_ID_a, 0)));
  return rb_str_new2(buf);
}


// -------------------------------------------------------------------------- //
// rb_Color_gray
// -------------------------------------------------------------------------- //
VALUE rb_Color_gray(VALUE self) {
  return C2R_INT(COLOR_TO_GRAYSCALE(R2C_INT(rb_funcall(self, rb_ID_r, 0)), R2C_INT(rb_funcall(self, rb_ID_g, 0)), R2C_INT(rb_funcall(self, rb_ID_b, 0))));
}

// -------------------------------------------------------------------------- //
// colorref_new
// -------------------------------------------------------------------------- //
ColorRef* colorref_new(Image* image, VALUE rb_image, int x, int y) {
  ColorRef* colorref;
  colorref = ALLOC(ColorRef); // ALLOC always succeeds or throws an exception
  colorref->image = image;
  colorref->rb_image = rb_image;
  colorref->x = x;
  colorref->y = y;
  return colorref;
}


// -------------------------------------------------------------------------- //
// colorref_destroy
// -------------------------------------------------------------------------- //
void colorref_destroy(ColorRef* colorref) {
  assert(colorref != NULL);

  xfree(colorref); // we must use xfree here, since we allocated colorref with ALLOC
}


// -------------------------------------------------------------------------- //
// colorref_mark
// -------------------------------------------------------------------------- //
void colorref_mark(ColorRef* colorref) {
  assert(colorref != NULL);

  rb_gc_mark(colorref->rb_image);
}

// -------------------------------------------------------------------------- //
// rb_ColorRef_check_raise
// -------------------------------------------------------------------------- //
VALUE rb_ColorRef_check_raise(VALUE self) {
  ColorRef* colorref;

  colorref = Data_Get_Struct_Ret(self, ColorRef);

  if(colorref->x < 0 || colorref->y < 0 || colorref->x >= image_width(colorref->image) || colorref->y >= image_height(colorref->image))
    rb_raise(rb_eRangeError, "trying to access pixel outside of image boundaries: (%d, %d), while operating on %dx%d image", colorref->x, colorref->y, 
      image_width(colorref->image), image_height(colorref->image));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_ColorRef_set
// -------------------------------------------------------------------------- //
VALUE rb_ColorRef_set(VALUE self, VALUE other) {
  Color color;
  color.r = rb_funcall(other, rb_ID_r, 0);
  color.g = rb_funcall(other, rb_ID_g, 0);
  color.b = rb_funcall(other, rb_ID_b, 0);
  color.a = rb_funcall(other, rb_ID_a, 0);
  rb_ColorRef_setter(self, &color);
  return self;
}


// -------------------------------------------------------------------------- //
// rb_ColorRef_r/g/b/a
// -------------------------------------------------------------------------- //
#define DEFINE_COLORREF_GETTER(SUFFIX)                                          \
VALUE rb_ColorRef_ ## SUFFIX(VALUE self) {                                      \
  Color color;                                                                  \
  return C2R_INT(rb_ColorRef_getter(self, &color)-> SUFFIX);                    \
}

DEFINE_COLORREF_GETTER(r)
DEFINE_COLORREF_GETTER(g)
DEFINE_COLORREF_GETTER(b)
DEFINE_COLORREF_GETTER(a)


// -------------------------------------------------------------------------- //
// rb_ColorRef_r/g/b/a_eq
// -------------------------------------------------------------------------- //
#define DEFINE_COLORREF_SETTER(SUFFIX)                                          \
VALUE rb_ColorRef_ ## SUFFIX ## _eq(VALUE self, VALUE val) {                    \
  Color color;                                                                  \
  rb_ColorRef_getter(self, &color)-> SUFFIX = R2C_INT(val);                     \
  rb_ColorRef_setter(self, &color);                                             \
  return val;                                                                   \
}

DEFINE_COLORREF_SETTER(r)
DEFINE_COLORREF_SETTER(g)
DEFINE_COLORREF_SETTER(b)
DEFINE_COLORREF_SETTER(a)

