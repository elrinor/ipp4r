#include "ipp4r_struct.h"

// -------------------------------------------------------------------------- //
// ippi_size
// -------------------------------------------------------------------------- //
IppiSize ippi_size(int width, int height) {
  IppiSize result;
  result.height = height;
  result.width = width;
  return result;
}


// -------------------------------------------------------------------------- //
// rb_Size_alloc
// -------------------------------------------------------------------------- //
VALUE rb_Size_alloc(VALUE klass) {
  return Data_Wrap_Struct(klass, NULL, assert_not_null_and_free, NULL);
}


// -------------------------------------------------------------------------- //
// rb_Size_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Size_initialize(int argc, VALUE *argv, VALUE self) {
  IppiSize* ippiSize;

  if(argc != 2)
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 2)", argc);

  ippiSize = malloc(sizeof(IppiSize));
  if(ippiSize == NULL)
    rb_raise(rb_eNoMemError, "could not allocate IppiSize structure");

  ippiSize->width  = R2C_INT(argv[0]);
  ippiSize->height = R2C_INT(argv[1]);

  DATA_PTR(self) = ippiSize;

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Size_to_s
// -------------------------------------------------------------------------- //
VALUE rb_Size_to_s(VALUE self) {
  IppiSize* ippiSize;
  char buf[100]; // I guess 100 is enough %)

  ippiSize = Data_Get_Struct_Ret(self, IppiSize);

  sprintf(buf, "%dx%d", ippiSize->width, ippiSize->height);
  return rb_str_new2(buf);
}


// -------------------------------------------------------------------------- //
// ippi_point
// -------------------------------------------------------------------------- //
IppiPoint ippi_point(int x, int y) {
  IppiPoint result;
  result.x = x;
  result.y = y;
  return result;
}


// -------------------------------------------------------------------------- //
// rb_Point_alloc
// -------------------------------------------------------------------------- //
VALUE rb_Point_alloc(VALUE klass) {
  return Data_Wrap_Struct(klass, NULL, assert_not_null_and_free, NULL);
}


// -------------------------------------------------------------------------- //
// rb_Point_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Point_initialize(int argc, VALUE *argv, VALUE self) {
  IppiPoint* ippiPoint;

  if(argc != 2)
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 2)", argc);

  ippiPoint = malloc(sizeof(IppiPoint));
  if(ippiPoint == NULL)
    rb_raise(rb_eNoMemError, "could not allocate IppiPoint structure");

  ippiPoint->x = R2C_INT(argv[0]);
  ippiPoint->y = R2C_INT(argv[1]);

  DATA_PTR(self) = ippiPoint;

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Point_to_s
// -------------------------------------------------------------------------- //
VALUE rb_Point_to_s(VALUE self) {
  IppiPoint* ippiPoint;
  char buf[100]; // I guess 100 is enough %)

  ippiPoint = Data_Get_Struct_Ret(self, IppiPoint);

  sprintf(buf, "(%d, %d)", ippiPoint->x, ippiPoint->y);
  return rb_str_new2(buf);
}
