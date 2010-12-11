#ifndef __IPP4R_COLOR_H__
#define __IPP4R_COLOR_H__

#include <ruby.h>
#include "ipp4r_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------- //
// Typedefs
// -------------------------------------------------------------------------- //
/**
 * Color struct
 */
struct _Color {
  int r, g, b, a;
};


/**
 * ColorRef struct
 */
struct _ColorRef {
  VALUE rb_image;
  Image* image;
  int x;
  int y;
};


// -------------------------------------------------------------------------- //
// Color C interface
// -------------------------------------------------------------------------- //
/**
 * Allocates memory for Color and initializes it.
 *  
 * @returns newly allocated Color, or throws a ruby exception in case of an error.
 */
Color* color_new(int red, int green, int blue, int alpha);


/**
 * Frees memory occupied by Color structure.
 */
void color_destroy(Color* color);


// -------------------------------------------------------------------------- //
// Color ruby interface
// -------------------------------------------------------------------------- //
/**
 * Alloc function for Color class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Color_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Color#initialize(red, green, blue, alpha = 255)</tt>
 * <li> <tt>Ipp::Color#initialize(gray = 0)</tt>
 * </ul>
 *         
 * Initializes a new Color object.
 */
VALUE rb_Color_initialize(int argc, VALUE *argv, VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Color#to_s</tt>
 * <li> <tt>Ipp::ColorRef#to_s</tt>
 * </ul>
 *         
 * @returns string representation of color
 */
VALUE rb_Color_to_s(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Color#gray</tt>
 * <li> <tt>Ipp::Image::ColorRef#gray</tt>
 * </ul>
 * 
 * @returns color value in grayscale
 */
VALUE rb_Color_gray(VALUE self);


// -------------------------------------------------------------------------- //
// ColorRef C interface
// -------------------------------------------------------------------------- //
/**
 * Allocates memory for ColorRef and initializes it.
 *
 * @returns newly allocated ColorRef, or throws a ruby exception in case of an error.
 */
ColorRef* colorref_new(Image* image, VALUE rb_image, int x, int y);


/**
* Frees memory occupied by ColorRef structure.
 */
void colorref_destroy(ColorRef* colorref);


/**
 * Mark function for ruby mark'n'sweep gc
 */
void colorref_mark(ColorRef* colorref);


// -------------------------------------------------------------------------- //
// ColorRef ruby interface
// -------------------------------------------------------------------------- //
/**
 * Method: 
 * <ul>
 * <li> <tt><i>none</i></tt>
 * </ul>
 * 
 * Checks whether the given ColorRef references a valid pixel, and throws a ruby exception if not.
 * @returns self
 */
VALUE rb_ColorRef_check_raise(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt><i>none</i></tt>
 * </ul>
 * 
 * "operator=" for ColorRef.
 * other param must be of Color duck type.
 * @returns self
 */
VALUE rb_ColorRef_set(VALUE self, VALUE other);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#r</tt>
 * </ul>
 * 
 * @returns color red component
 */
VALUE rb_ColorRef_r(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#g</tt>
 * </ul>
 * 
 * @returns color green component
 */
VALUE rb_ColorRef_g(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#b</tt>
 * </ul>
 * 
 * @returns color blue component
 */
VALUE rb_ColorRef_b(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#a</tt>
 * </ul>
 * 
 * @returns color alpha component
 */
VALUE rb_ColorRef_a(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#r=</tt>
 * </ul>
 * 
 * Setter for red color component
 */
VALUE rb_ColorRef_r_eq(VALUE self, VALUE val);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#g=</tt>
 * </ul>
 * 
 * Setter for green color component
 */
VALUE rb_ColorRef_g_eq(VALUE self, VALUE val);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#b=</tt>
 * </ul>
 * 
 * Setter for blue color component
 */
VALUE rb_ColorRef_b_eq(VALUE self, VALUE val);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::ColorRef#a=</tt>
 * </ul>
 * 
 * Setter for alpha color component
 */
VALUE rb_ColorRef_a_eq(VALUE self, VALUE val);


// -------------------------------------------------------------------------- //
// Macros
// -------------------------------------------------------------------------- //
#define WRAP_COLORREF_A(COLORREF, CLASS)                                        \
  Data_Wrap_Struct((CLASS), colorref_mark, colorref_destroy, (COLORREF))

#define WRAP_COLORREF(COLORREF)                                                 \
  WRAP_COLORREF_A(COLORREF, rb_ColorRef)

#define COLOR_TO_GRAYSCALE(R, G, B)                                             \
  ((int) (0.299f * (R) + 0.587f * (G) + 0.114f * (B)))


#ifdef __cplusplus
}
#endif

#endif
