#ifndef __IPP4R_COLOR_H__
#define __IPP4R_COLOR_H__

#include <ruby.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Color {
  int red, green, blue, alpha;
} Color;


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
 * </ul>
 *         
 * @returns string representation of Ipp::Color
 */
VALUE rb_Color_to_s(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Color#gray</tt>
 * <li> <tt>Ipp::Image::ColorRef#gray</tt>
 * </ul>
 * 
 * @returns color value in grayscale.
 */
VALUE rb_Color_gray(VALUE self);


#ifdef __cplusplus
}
#endif

#endif
