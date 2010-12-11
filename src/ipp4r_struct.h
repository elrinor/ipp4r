#ifndef __IPP4R_STRUCT_H__
#define __IPP4R_STRUCT_H__

#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// IppiSize
// -------------------------------------------------------------------------- //
typedef IppiSize Size;

/**
 * IppiSize constructor
 */
IppiSize ippi_size(int width, int height);


/**
 * Alloc function for Size class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Size_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Size#initialize(width, height)</tt>
 * </ul>
 *         
 * Initializes a new Size object.
 */
VALUE rb_Size_initialize(int argc, VALUE *argv, VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Size#to_s</tt>
 * </ul>
 *         
 * @returns string representation of Size
 */
VALUE rb_Size_to_s(VALUE self);


// -------------------------------------------------------------------------- //
// IppiPoint
// -------------------------------------------------------------------------- //
typedef IppiPoint Point;

/**
 * IppiPoint constructor
 */
IppiPoint ippi_point(int x, int y);


/**
 * Alloc function for Point class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Point_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Point#initialize(x, y)</tt>
 * </ul>
 *         
 * Initializes a new Point object.
 */
VALUE rb_Point_initialize(int argc, VALUE *argv, VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Point#to_s</tt>
 * </ul>
 *         
 * @returns string representation of Point
 */
VALUE rb_Point_to_s(VALUE self);




#endif

