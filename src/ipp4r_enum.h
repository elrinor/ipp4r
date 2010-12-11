#ifndef __IPP4R_ENUM_H__
#define __IPP4R_ENUM_H__

#include <ruby.h>
#include "ipp4r.h" /* for rb_ID_* */

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------- //
// Typedefs & Enums
// -------------------------------------------------------------------------- //
/**
 * Enumeration struct, for emulating enums in ruby.
 */
typedef struct _Enum {
  ID id;
  int value;
} Enum;


// -------------------------------------------------------------------------- //
// Enum helpers
// -------------------------------------------------------------------------- //
/**
 * Sets up a subclass of Enum
 */
VALUE define_enum_type(char *tag);


/**
 * Constructs a new Enum subclass instance
 */
VALUE enum_new(VALUE klass, VALUE sym, VALUE value);


// -------------------------------------------------------------------------- //
// Enum class ruby interface
// -------------------------------------------------------------------------- //
/**
 * Alloc function for Enum class.
 */
VALUE rb_Enum_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Enum#initialize(symbol, value)</tt>
 * </ul>
 *         
 * Initializes a new Enum object.
 * @returns self
 */
VALUE rb_Enum_initialize(VALUE self, VALUE sym, VALUE val);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Enum#to_s</tt>
 * </ul>
 *         
 * @returns string representation of a Ipp::Enum
 */
VALUE rb_Enum_to_s(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Enum#to_i</tt>
 * </ul>
 *         
 * @returns value of Ipp::Enum
 */
VALUE rb_Enum_to_i(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Enum#<=></tt>
 * </ul>
 * 
 * Support Comparable module in Enum.
 * Enums must be of the same class to be equal.
 * @returns -1, 0, 1, or Qnil
 */
VALUE rb_Enum_spaceship(VALUE self, VALUE other);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Enum#===</tt>
 * </ul>
 * 
 * "Case equal" operator for Enum
 * @returns Qtrue or Qfalse
 */
VALUE rb_Enum_case_eq(VALUE self, VALUE other);


// -------------------------------------------------------------------------- //
// Enum types ruby interface
// -------------------------------------------------------------------------- //
/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::XXX#initialize</tt>
 * </ul>
 * 
 * initialize method for Enum subclasses
 * @returns self
 */
VALUE rb_Enum_type_initialize(VALUE self, VALUE sym, VALUE val);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::XXX#inspect</tt>
 * </ul>
 * 
 * inspect method for Enum subclasses
 * @returns string representation of an Enum subclass value
 */
VALUE rb_Enum_type_inspect(VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::XXX#values</tt>
 * </ul>
 * 
 * Behaves like #each if a block is present, otherwise like #to_a
 */
VALUE rb_Enum_type_values(VALUE klass);


// -------------------------------------------------------------------------- //
// Defines
// -------------------------------------------------------------------------- //
#define ENUM_DEF(C_NAME, R_NAME_STR)                                            \
  do {                                                                          \
    VALUE klass;                                                                \
    klass = C_NAME = define_enum_type(R_NAME_STR);

#define ENUM(C_NAME, R_NAME_STR)                                                \
    rb_define_const(rb_Ipp, R_NAME_STR, enum_new(klass, ID2SYM(rb_intern(R_NAME_STR)), INT2FIX(C_NAME)));

#define ENUM_END()                                                              \
    rb_funcall(klass, rb_ID_private_class_method, 1, ID2SYM(rb_ID_new));        \
  } while(0);

#define R2C_ENUM(VAL, R_TYPE)                                                   \
  ((CLASS_OF(VAL) != R_TYPE)?                                                   \
    ((long) rb_raise_ret(rb_eTypeError, "wrong enumeration type - expected %s, got %s", rb_class2name(R_TYPE), rb_class2name(CLASS_OF(VAL)))):\
    (Data_Get_Struct_Ret(VAL, Enum)->value)                                     \
  )

#define C2R_ENUM(VAL, R_TYPE)                                                   \
  assert_not_qnil_ret(rb_hash_aref(rb_cvar_get(R_TYPE, rb_ID_enumerators_by_value), INT2FIX(VAL)))


#ifdef __cplusplus
}
#endif

#endif