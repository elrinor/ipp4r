#ifndef __IPP4R_COLOR_H__
#define __IPP4R_COLOR_H__

#include <ruby.h>
#include <ippdefs.h>
#include "ipp4r_fwd.h"
#include "ipp4r_metatype.h"

/**
 * @file 
 *
 * This file defines C and Ruby interfaces for working with colors. <p>
 *
 * One of the main concepts of color infrastructure is the so-called "MetaColor". 
 * The c-type for MetaColor is IppMetaNumber[4] - pointer to an array of at least 4 IppMetaNumbers.
 * The MetaColor of Color structure can be accessed through as_array field. <p>
 *
 * Also this header defines some useful macros for color conversion between different color types, i.e. grayscale, rgb, rgba, and between colors of different data types. The latter
 * relies on IppMetaType conversion infrastructure. Color conversion functions exist in 2 variants - expression-type and statement-type converters. Expression-type converters
 * use color slots for intermediate data storage, and that's why user must allocate some slots with macro USING_M2C_COLOR before using these functions.
 */

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
 * Allocates memory for Color and initializes it. <br>
 * Note that r, g, b and a parameters must be in [0, 1] range.
 *
 * @param r red color component
 * @param g green color component
 * @param b blue color component
 * @param a alpha color component
 * @returns newly allocated Color, or NULL in case of an error.
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
 * @returns newly allocated ColorRef, or NULL in case of an error.
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
/**
 * Wraps a ColorRef structure into ruby VALUE using given CLASS
 *
 * @param COLORREF <tt>ColorRef*</tt>
 * @param CLASS <tt>VALUE</tt> of ruby class used for wrapping
 */
#define WRAP_COLORREF_A(COLORREF, CLASS)                                        \
  Data_Wrap_Struct((CLASS), colorref_mark, colorref_destroy, (COLORREF))


/**
 * Wraps a ColorRef structure into ruby VALUE.
 *
 * @param COLORREF <tt>ColorRef*</tt>
 */
#define WRAP_COLORREF(COLORREF)                                                 \
  WRAP_COLORREF_A(COLORREF, rb_ColorRef)


/**
 * Converts the given ruby color to C Color.
 * 
 * @param C_COLOR <tt>Color*</tt>
 * @param R_COLOR <tt>VALUE</tt> of ruby color
 */
#define R2C_COLOR(C_COLOR, R_COLOR) do {                                        \
    (C_COLOR)->r = R2M_NUM(rb_funcall((R_COLOR), rb_ID_r, 0));                  \
    (C_COLOR)->g = R2M_NUM(rb_funcall((R_COLOR), rb_ID_g, 0));                  \
    (C_COLOR)->b = R2M_NUM(rb_funcall((R_COLOR), rb_ID_b, 0));                  \
    (C_COLOR)->a = R2M_NUM(rb_funcall((R_COLOR), rb_ID_a, 0));                  \
  } while(0)


// -------------------------------------------------------------------------- //
// Color conversion
// -------------------------------------------------------------------------- //
/**
 * Converts the given RGB triad to grayscale.
 */
#define COLOR_TO_GRAYSCALE(R, G, B)                                             \
  (0.299f * (R) + 0.587f * (G) + 0.114f * (B))


/**
 * Converts the given rgb color array to grayscale.
 */
#define COLORARRAY_TO_GRAYSCALE(COLORARRAY)                                      \
  COLOR_TO_GRAYSCALE((COLORARRAY)[2], (COLORARRAY)[1], (COLORARRAY)[0]) /* BGR */


// -------------------------------------------------------------------------- //
// Color meta conversion
// -------------------------------------------------------------------------- //
#define M2C_COLOR_STORAGE(DATATYPE, SLOT)                                       \
  ARX_JOIN_4(m2c_color_, DATATYPE, _, SLOT)


/**
 * Allocates SLOTS for intermediate data storage for expression-type color conversion functions.
 *
 * @param SLOTS number of conversion slots to allocate
 */
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


/**
 * Statement-type converter from MetaColor to the color type defined by METATYPE.
 *
 * @param METATYPE type to convert the given color to
 * @param FROM_METACOLOR MetaColor to convert
 * @param TO_COLOR target color to store result to
 */
#define M2C_COLOR_TO(METATYPE, FROM_METACOLOR, TO_COLOR)                        \
  ( M2C_COLOR_TO_I(METATYPE, M_DATATYPE(METATYPE), M_CHANNELS(METATYPE), (FROM_METACOLOR), (TO_COLOR)) )
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
 * Statement-type converter from the color type defined by METATYPE to MetaColor.
 *
 * @param METATYPE type to convert the given color from
 * @param FROM_COLOR color to convert
 * @param TO_METACOLOR target MetaColor to store result to
 */
#define C2M_COLOR_TO(METATYPE, FROM_COLOR, TO_METACOLOR)                        \
  ( C2M_COLOR_TO_I(METATYPE, M_DATATYPE(METATYPE), M_CHANNELS(METATYPE), (FROM_COLOR), (TO_METACOLOR)) )
#define C2M_COLOR_TO_I(METATYPE, DATATYPE, CHANNELS, FROM_COLOR, TO_METACOLOR)  \
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(IF_C_EQ_C(CHANNELS, C1, 3, C_CNUMB(CHANNELS))), C2M_COLOR_TO_II, (4, (DATATYPE, CHANNELS, FROM_COLOR, TO_METACOLOR))) \
  ARX_IF(ARX_NOT_EQUAL(C_CNUMB(CHANNELS), 4), TO_METACOLOR[3] = 1.0, NULL)
#define C2M_COLOR_TO_II(INDEX, ARGS)                                            \
  C2M_COLOR_TO_III(INDEX, ARX_ARRAY_ELEM(0, ARGS), ARX_ARRAY_ELEM(1, ARGS), ARX_ARRAY_ELEM(2, ARGS), ARX_ARRAY_ELEM(3, ARGS))
#define C2M_COLOR_TO_III(INDEX, DATATYPE, CHANNELS, FROM_COLOR, TO_METACOLOR) \
  ( TO_METACOLOR[INDEX] = C2M_NUMBER_D(DATATYPE, ((D_CTYPE(DATATYPE)*)FROM_COLOR)[IF_C_EQ_C(CHANNELS, C1, 0, INDEX)]) ),


/**
 * Expression-type converter from MetaColor to the color type defined by METATYPE. Stores intermediate data in the given SLOT.
 *
 * @param METATYPE type to convert the given color to
 * @param METACOLOR MetaColor to convert
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
  (                                                                             \
    M2C_COLOR_TO(METATYPE, METACOLOR, M2C_COLOR_STORAGE(DATATYPE, SLOT)),       \
    M2C_COLOR_STORAGE(DATATYPE, SLOT)                                           \
  )

#ifdef __cplusplus
}
#endif

#endif
