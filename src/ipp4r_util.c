#include <assert.h>
#include <ruby.h>
#include <stdarg.h> /* for va_list */
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// get_struct_checked_ret
// -------------------------------------------------------------------------- //
void* get_struct_checked_ret(VALUE obj) {
  void* result;
  Data_Get_Struct(obj, void, result);
  return result;
}


// -------------------------------------------------------------------------- //
// rb_raise_ret
// -------------------------------------------------------------------------- //
VALUE rb_raise_ret(VALUE exc, const char* fmt, ...) {
  va_list args;
  char buf[1024]; // I guess that'd be enough

  va_start(args, fmt);
  vsnprintf(buf, 1024, fmt, args);
  va_end(args);
  rb_exc_raise(rb_exc_new2(exc, buf));

  return Qnil; // to make compiler happy
}


// -------------------------------------------------------------------------- //
// assert_not_qnil_ret
// -------------------------------------------------------------------------- //
VALUE assert_not_qnil_ret(VALUE arg) {
  assert(arg != Qnil);
  return arg;
}


// -------------------------------------------------------------------------- //
// assert_not_null_and_free
// -------------------------------------------------------------------------- //
void assert_not_null_and_free(void* p) {
  assert(p != NULL);

  free(p);
}


// -------------------------------------------------------------------------- //
// raise_on_error
// -------------------------------------------------------------------------- //
void raise_on_error(int status) {
  if(IS_ERROR(status))
    rb_raise(rb_Exception, "%s", image_error_message(status));
  if(IS_WARNING(status))
    rb_warning("%s", image_error_message(status));
}

