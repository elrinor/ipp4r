#ifndef __IPP4R_COLOR_H__
#define __IPP4R_COLOR_H__

#include <ruby.h>
#include <ippdefs.h>
#include "ipp4r_fwd.h"
#include "ipp4r_metatype.h"

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
  union {
    IppMetaNumber as_array[4];
    struct {
      IppMetaNumber b, g, r, a;
    };
  };
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
 * Allocates memory for Color and initializes it. <br/>
 * Note that r, g, b and a parameters must be in [0, 1] range.
 *
 * @param r red color component
 * @param g green color component
 * @param b blue color component
 * @param a alpha color component
 * @returns newly allocated Color, or throws a ruby exception in case of an error.
 */
Color* color_new(IppMetaNumber r, IppMetaNumber g, IppMetaNumber b, IppMetaNumber a);


/**
 * Frees memory occupied by Color structure.
 */
void color_destroy(Color* color);


/**
 * @returns color value in grayscale
 */
/*Ipp32f color_gray(Color* color);*/


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
 * <li> <tt>Ipp::Color#initialize(red, green, blue, alpha = 1.0)</tt>
 * <li> <tt>Ipp::Color#initialize(gray = 0.0)</tt>
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
/*VALUE rb_Color_gray(VALUE self);*/


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
 * "operator=" for ColorRef.
 * other param must be of Color duck type.
 * 
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

#define R2C_COLOR(C_COLOR, R_COLOR) do {                                        \
    (C_COLOR).r = (IppMetaNumber) R2C_METANUM(rb_funcall((R_COLOR), rb_ID_r, 0)); \
    (C_COLOR).g = (IppMetaNumber) R2C_METANUM(rb_funcall((R_COLOR), rb_ID_g, 0)); \
    (C_COLOR).b = (IppMetaNumber) R2C_METANUM(rb_funcall((R_COLOR), rb_ID_b, 0)); \
    (C_COLOR).a = (IppMetaNumber) R2C_METANUM(rb_funcall((R_COLOR), rb_ID_a, 0)); \
  } while(0)


// -------------------------------------------------------------------------- //
// Color conversion
// -------------------------------------------------------------------------- //
#define COLOR_TO_GRAYSCALE(R, G, B)                                             \
  (0.299f * (R) + 0.587f * (G) + 0.114f * (B))

#define COLORARRAY_TO_GRAYSCALE(COLORARRAY)                                      \
  COLOR_TO_GRAYSCALE((COLORARRAY)[2], (COLORARRAY)[1], (COLORARRAY)[0]) /* BGR */


// -------------------------------------------------------------------------- //
// Color meta conversion
// -------------------------------------------------------------------------- //
#define M2C_COLOR_STORAGE(DATATYPE, SLOT)                                       \
  ARX_JOIN_4(m2c_color_, DATATYPE, _, SLOT)

#define USING_M2C_COLOR(SLOTS)                                                  \
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(SLOTS), USING_M2C_COLOR_D, ~)               \
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(SLOTS), USING_M2C_COLOR_U, ~)
#define USING_M2C_COLOR_D(SLOT, ARGS)                                           \
  ARX_ARRAY_FOREACH(D_SUPPORTED, USING_M2C_COLOR_DI, SLOT)
#define USING_M2C_COLOR_DI(DATATYPE, SLOT)                                      \
  IF_D_EQ_D(DATATYPE, D_METANUM, ARX_EMPTY(), D_CTYPE(DATATYPE) M2C_COLOR_STORAGE(DATATYPE, SLOT)[C_MAX_COUNT];)
#define USING_M2C_COLOR_U(SLOT, ARGS)                                           \
  ARX_ARRAY_FOREACH(D_SUPPORTED, USING_M2C_COLOR_UI, SLOT)
#define USING_M2C_COLOR_UI(DATATYPE, SLOT)                                      \
  IF_D_EQ_D(DATATYPE, D_METANUM, ARX_EMPTY(), (void) M2C_COLOR_STORAGE(DATATYPE, SLOT);) /* just to make the compiler happy */


#define M2C_COLOR_TO(METATYPE, FROM_METACOLOR, TO_COLOR)                        \
  M2C_COLOR_TO_I(METATYPE, M_DATATYPE(METATYPE), M_CHANNELS(METATYPE), (FROM_METACOLOR), (TO_COLOR))
#define M2C_COLOR_TO_I(METATYPE, DATATYPE, CHANNELS, FROM_METACOLOR, TO_COLOR)  \
  IF_C_EQ_C(CHANNELS, C1,                                                       \
    ((D_CTYPE(DATATYPE)*)TO_COLOR)[0] = M2C_NUMBER_D(DATATYPE, COLORARRAY_TO_GRAYSCALE(FROM_METACOLOR)) ARX_TUPLE_EAT_5, \
    M2C_COLOR_TO_II \
  )(METATYPE, DATATYPE, CHANNELS, FROM_METACOLOR, TO_COLOR)
#define M2C_COLOR_TO_II(METATYPE, DATATYPE, CHANNELS, FROM_METACOLOR, TO_COLOR) \
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(C_CNUMB(CHANNELS)), M2C_COLOR_TO_III, (4, (DATATYPE, CHANNELS, FROM_METACOLOR, TO_COLOR))) NULL
#define M2C_COLOR_TO_III(INDEX, ARGS)                                           \
  M2C_COLOR_TO_OO(INDEX, ARX_ARRAY_ELEM(0, ARGS), ARX_ARRAY_ELEM(1, ARGS), ARX_ARRAY_ELEM(2, ARGS), ARX_ARRAY_ELEM(3, ARGS))
#define M2C_COLOR_TO_OO(INDEX, DATATYPE, CHANNELS, FROM_METACOLOR, TO_COLOR)             \
  ( ((D_CTYPE(DATATYPE)*)TO_COLOR)[INDEX] = M2C_NUMBER_D(DATATYPE, FROM_METACOLOR[INDEX]) ),


/**
 * @param METACOLOR color array, must be a pointer of type <tt>IppMetaNumber*</tt>
 * @param METATYPE type to convert the given color to
 * @param SLOT conversion slot to use
 */
#define M2C_COLOR(METATYPE, METACOLOR, SLOT)                                    \
  M2C_COLOR_I(METATYPE, M_DATATYPE(METATYPE), M_CHANNELS(METATYPE), (METACOLOR), SLOT)
#define M2C_COLOR_I(METATYPE, DATATYPE, CHANNELS, METACOLOR, SLOT)              \
  IF_C_EQ_C(CHANNELS, C1,                                                       \
    M2C_NUMBER_D(DATATYPE, COLORARRAY_TO_GRAYSCALE(METACOLOR)),                 \
    IF_D_EQ_D(DATATYPE, D_METANUM,                                              \
      METACOLOR ARX_TUPLE_EAT_5,                                                \
      M2C_COLOR_II                                                              \
    )(METATYPE, DATATYPE, CHANNELS, METACOLOR, SLOT)                            \
  )
#define M2C_COLOR_II(METATYPE, DATATYPE, CHANNELS, METACOLOR, SLOT)             \
  ( ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(C_CNUMB(CHANNELS)), M2C_COLOR_III, (3, (DATATYPE, METACOLOR, SLOT))) \
  M2C_COLOR_STORAGE(DATATYPE, SLOT) )
#define M2C_COLOR_III(INDEX, ARGS)                                              \
  M2C_COLOR_OO(INDEX, ARX_ARRAY_ELEM(0, ARGS), ARX_ARRAY_ELEM(1, ARGS), ARX_ARRAY_ELEM(2, ARGS))
#define M2C_COLOR_OO(INDEX, DATATYPE, METACOLOR, SLOT)                          \
  (M2C_COLOR_STORAGE(DATATYPE, SLOT)[INDEX] = M2C_NUMBER_D(DATATYPE, METACOLOR[INDEX])),


#ifdef __cplusplus
}
#endif

#endif
