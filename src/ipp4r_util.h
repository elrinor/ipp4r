#ifndef __IPP4R_UTIL_H__
#define __IPP4R_UTIL_H__

#include <ruby.h>

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------- //
// Function prototypes
// -------------------------------------------------------------------------- //
/**
 * @returns TRUE if the given IppChannels enum is supported, FALSE otherwise
 */
int is_channels_supported(IppChannels channels);


/**
 * @returns TRUE if the given IppDataType is supported, FALSE otherwise
 */
int is_datatype_supported(IppDataType dataType);


/**
 * @return size in bytes of one pixel for given channels
 */
int pixel_size(IppChannels channels);


/**
 * Replacement for ruby's Data_Get_Struct macro. <br/>
 * Rationale: this is a function with return value => it can be used in expressions, unlike Data_Get_Struct, which is a statement. "Syntactic sugar", that's it.
 */
void* get_struct_checked_ret(VALUE obj); 


/**
 * Replacement for ruby's rb_raise. <br/>
 * Rationale: same as for get_struct_checked_ret. This is a function with return value => it can be used in expressions.
 *
 * @returns Qnil
 */
VALUE rb_raise_ret(VALUE exc, const char* fmt, ...);


/**
 * asserts arg != Qnil. <br/>
 * Just Another Syntactic Sugar Function (c) 
 *
 * @returns arg
 */
VALUE assert_not_qnil_ret(VALUE arg);


/**
 * Raises ruby exception if status indicates an error, writes a warning if status indicates a warning.
 */
void raise_on_error(int status);


// -------------------------------------------------------------------------- //
// Defines
// -------------------------------------------------------------------------- //
#define Data_Get_Struct_Ret(VAL, C_TYPE)                                        \
  ((C_TYPE*) get_struct_checked_ret(VAL))

#ifdef __cplusplus
}
#endif

#endif
