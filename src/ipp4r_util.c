#include <assert.h>
#include <ruby.h>
#include <stdarg.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// is_channels_supported
// -------------------------------------------------------------------------- //
int is_channels_supported(IppChannels channels) {
  IPPMETACALL(channels, return, C_SUPPORTED, IPPMETAFUNC, (C_INTERCEPT, ARX_EMPTY(), TRUE), ARX_EMPTY(), FALSE);
}

// -------------------------------------------------------------------------- //
// is_datatype_supported
// -------------------------------------------------------------------------- //
int is_datatype_supported(IppDataType dataType) {
  IPPMETACALL(dataType, return, D_SUPPORTED, IPPMETAFUNC, (D_INTERCEPT, ARX_EMPTY(), TRUE), ARX_EMPTY(), FALSE);
}


// -------------------------------------------------------------------------- //
// pixel_size
// -------------------------------------------------------------------------- //
int pixel_size(IppMetaType metaType) {
#define METAFUNC(M, ARGS) (sizeof(D_CTYPE(M_DATATYPE(M))) * C_CNUMB(M_CHANNELS(M)))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
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
