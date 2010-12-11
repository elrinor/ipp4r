#include <assert.h>
#include <ruby.h>
#include <stdarg.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// is_channels_supported
// -------------------------------------------------------------------------- //
int is_channels_supported(IppChannels channels) {
  int result;
  IPPMETACALL(channels, result, TRUE; ipp, ARX_EMPTY, (3, (C1, C3, AC4)), ARX_EMPTY, FALSE);
  return result;
}


// -------------------------------------------------------------------------- //
// pixel_size
// -------------------------------------------------------------------------- //
int pixel_size(IppChannels channels) {
  int pixelSize;

#define PIXELSIZE_C1 (sizeof(Ipp8u))
#define PIXELSIZE_C3 (3 * sizeof(Ipp8u))
#define PIXELSIZE_AC4 (4 * sizeof(Ipp8u))
  IPPMETACALL(channels, pixelSize, PIXELSIZE_, ARX_EMPTY, (3, (C1, C3, AC4)), ARX_EMPTY, -1; Unreachable() );

  return pixelSize;
}


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
// raise_on_error
// -------------------------------------------------------------------------- //
void raise_on_error(int status) {
  if(IS_ERROR(status))
    rb_raise(rb_Exception, "%s", image_error_message(status));
  if(IS_WARNING(status))
    rb_warning("%s", image_error_message(status));
}
