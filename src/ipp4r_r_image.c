#include "ipp4r.h"


// -------------------------------------------------------------------------- //
// rb_Image_alloc
// -------------------------------------------------------------------------- //
VALUE rb_Image_alloc(VALUE klass) {
  return WRAP_IMAGE_A(NULL, klass); /* Underlying C struct will be allocated later, in "initialize" method */
}


// -------------------------------------------------------------------------- //
// rb_Image_initialize
// -------------------------------------------------------------------------- //
VALUE rb_Image_initialize(int argc, VALUE *argv, VALUE self) {
  int width;
  int height;
  Image* image;
  int status;

  switch (argc) {
  case 2:
    width = NUM2INT(argv[0]);
    height = NUM2INT(argv[1]);
    if(width <= 0 || height <= 0)
      rb_raise(rb_Exception, "wrong image size: %d x %d", width, height);
    image = image_new(width, height, ippC3);
    break;
  case 1:
    Check_Type(argv[0], T_STRING);
    image = image_load(RSTRING_PTR(argv[0]), &status);
    raise_on_error(status);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 1 or 2)", argc);
    break;
  }

  if(image == NULL)
    rb_raise(rb_eNoMemError, "failed to create new image");

  DATA_PTR(self) = image;

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_save
// -------------------------------------------------------------------------- //
VALUE rb_Image_save(VALUE self, VALUE fileName) {
  Check_Type(fileName, T_STRING);

  raise_on_error(image_save(Data_Get_Struct_Ret(self, Image), RSTRING(fileName)->ptr));
  return Qnil;
}


// -------------------------------------------------------------------------- //
// rb_Image_add_rand_uniform_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_add_rand_uniform_bang(VALUE self, VALUE lo, VALUE hi) {
  raise_on_error(image_addranduniform(Data_Get_Struct_Ret(self, Image), NUM2INT(lo), NUM2INT(hi)));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_add_rand_uniform
// -------------------------------------------------------------------------- //
VALUE rb_Image_add_rand_uniform(VALUE self, VALUE lo, VALUE hi) {
  Image* newImage;
  int status;
  
  newImage = image_clone(Data_Get_Struct_Ret(self, Image), &status);

  raise_on_error(status);

  return rb_Image_add_rand_uniform_bang(WRAP_IMAGE(newImage), lo, hi);
}


// -------------------------------------------------------------------------- //
// rb_Image_width
// -------------------------------------------------------------------------- //
VALUE rb_Image_width(VALUE self) {
  Image* image;

  Data_Get_Struct(self, Image, image);

  return INT2NUM(image_width(image));
}


// -------------------------------------------------------------------------- //
// rb_Image_height
// -------------------------------------------------------------------------- //
VALUE rb_Image_height(VALUE self) {
  Image* image;

  Data_Get_Struct(self, Image, image);

  return INT2NUM(image_height(image));
}


// -------------------------------------------------------------------------- //
// rb_Image_channels
// -------------------------------------------------------------------------- //
VALUE rb_Image_channels(VALUE self) {
  Image* image;
  VALUE result;

  Data_Get_Struct(self, Image, image);
  
  return C2R_ENUM(image_channels(image), rb_Channels);
}


// -------------------------------------------------------------------------- //
// rb_Image_convert
// -------------------------------------------------------------------------- //
VALUE rb_Image_convert(VALUE self, VALUE r_channels) {
  Image* result;
  int status;

  result = image_convert_copy(Data_Get_Struct_Ret(self, Image), R2C_ENUM(r_channels, rb_Channels), &status);
  raise_on_error(status);

  return WRAP_IMAGE(result);
}


// -------------------------------------------------------------------------- //
// rb_Image_convert_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_convert_bang(VALUE self, VALUE r_channels) {
  raise_on_error(image_convert(Data_Get_Struct_Ret(self, Image), R2C_ENUM(r_channels, rb_Channels)));
  
  return self;
}



