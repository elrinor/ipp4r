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
  VALUE filler;

  filler = Qnil;

  switch (argc) {
  case 3:
    filler = argv[2];
  case 2:
    width = R2C_INT(argv[0]);
    height = R2C_INT(argv[1]);
    if(width <= 0 || height <= 0)
      rb_raise(rb_Exception, "wrong image size: %d x %d", width, height);
    image = image_new(width, height, ippC3);
    break;
  case 1:
    Check_Type(argv[0], T_STRING);
    image = image_load(R2C_STR(argv[0]), &status);
    raise_on_error(status);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 1 or 2)", argc);
    break;
  }

  if(image == NULL)
    rb_raise(rb_eNoMemError, "failed to create new image");

  DATA_PTR(self) = image;

  if(filler != Qnil)
    rb_Image_fill_bang(self, filler);

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_initialize_copy
// -------------------------------------------------------------------------- //
VALUE rb_Image_initialize_copy(VALUE self, VALUE other) {
  Image* newImage;
  int status;

  newImage = image_clone(Data_Get_Struct_Ret(other, Image), &status);

  raise_on_error(status);

  DATA_PTR(self) = newImage;

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_subimage
// -------------------------------------------------------------------------- //
VALUE rb_Image_subimage(int argc, VALUE *argv, VALUE self) {
  int x0, y0, x1, y1, width, height, t;
  Image* image;
  Image* newImage;
  int status;

  width = 0;
  height = 0;
  image = Data_Get_Struct_Ret(self, Image);

  switch(argc) {
  case 4:
    width = R2C_INT(argv[2]);
    height = R2C_INT(argv[3]);
  case 2:
    x0 = R2C_INT(argv[0]);
    y0 = R2C_INT(argv[1]);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 2 or 4)", argc);
    break;
  }

  if(width <= 0) {
    x1 = image_width(image) + width;
    width = x1 - x0;
  } else
    x1 = x0 + width;

  if(height <= 0) {
    y1 = image_height(image) + height;
    height = y1 - y0;
  } else
    y1 = y0 + height;

  if(width <= 0 || height <= 0)
    rb_raise(rb_eArgError, "wrong subimage size: %d x %d", width, height);

  if(x0 < 0 || y0 < 0 || x0 >= image_width(image) || y0 >= image_height(image))
    rb_raise(rb_eArgError, "wrong subimage upper-left corner coordinates: (%d, %d)", x0, y0);

  if(x1 <= 0 || y1 <= 0 || x1 > image_width(image) || y1 > image_height(image))
    rb_raise(rb_eArgError, "wrong subimage lower-right corner coordinates: (%d, %d)", x1, y1);

  newImage = image_subimage(image, x0, y0, width, height, &status);
  raise_on_error(status);

  return WRAP_IMAGE(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_save
// -------------------------------------------------------------------------- //
VALUE rb_Image_save(VALUE self, VALUE fileName) {
  Check_Type(fileName, T_STRING);

  raise_on_error(image_save(Data_Get_Struct_Ret(self, Image), R2C_STR(fileName)));
  return Qnil;
}


// -------------------------------------------------------------------------- //
// rb_Image_add_rand_uniform_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_add_rand_uniform_bang(VALUE self, VALUE lo, VALUE hi) {
  raise_on_error(image_addranduniform(Data_Get_Struct_Ret(self, Image), R2C_INT(lo), R2C_INT(hi)));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_add_rand_uniform
// -------------------------------------------------------------------------- //
VALUE rb_Image_add_rand_uniform(VALUE self, VALUE lo, VALUE hi) {
  return rb_Image_add_rand_uniform_bang(rb_funcall(self, rb_ID_clone, 0), lo, hi);
}


// -------------------------------------------------------------------------- //
// rb_Image_width
// -------------------------------------------------------------------------- //
VALUE rb_Image_width(VALUE self) {
  Image* image;

  Data_Get_Struct(self, Image, image);

  return C2R_INT(image_width(image));
}


// -------------------------------------------------------------------------- //
// rb_Image_height
// -------------------------------------------------------------------------- //
VALUE rb_Image_height(VALUE self) {
  Image* image;

  Data_Get_Struct(self, Image, image);

  return C2R_INT(image_height(image));
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
  /*raise_on_error*/(image_convert(Data_Get_Struct_Ret(self, Image), R2C_ENUM(r_channels, rb_Channels)));
  
  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_ref
// -------------------------------------------------------------------------- //
VALUE rb_Image_ref(VALUE self, VALUE x, VALUE y) {
  return rb_ColorRef_check_raise(WRAP_COLORREF(colorref_new(Data_Get_Struct_Ret(self, Image), self, R2C_INT(x), R2C_INT(y))));
}


// -------------------------------------------------------------------------- //
// rb_Image_ref_eq
// -------------------------------------------------------------------------- //
VALUE rb_Image_ref_eq(VALUE self, VALUE x, VALUE y, VALUE color) {
  return rb_ColorRef_set(rb_Image_ref(self, x, y), color);
}


// -------------------------------------------------------------------------- //
// rb_Image_fill_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_fill_bang(VALUE self, VALUE rb_color) {
  Color color;
  Image* image;

  image = Data_Get_Struct_Ret(self, Image);

  R2C_COLOR(color, rb_color);

  raise_on_error(image_fill(image, &color));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_fill
// -------------------------------------------------------------------------- //
VALUE rb_Image_fill(VALUE self, VALUE color) {
  return rb_Image_fill_bang(rb_funcall(self, rb_ID_clone, 0), color);
}


// -------------------------------------------------------------------------- //
// rb_Image_transpose
// -------------------------------------------------------------------------- //
VALUE rb_Image_transpose(VALUE self) {
  Image* newImage;
  int status;

  newImage = image_transpose_copy(Data_Get_Struct_Ret(self, Image), &status);
  raise_on_error(status);

  return WRAP_IMAGE(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_threshold
// -------------------------------------------------------------------------- //
VALUE rb_Image_threshold(int argc, VALUE* argv, VALUE self) {
  Image* newImage;
  int status;
  IppCmpOp cmp;
  Color threshold, value;
 
  switch(argc) {
  case 1:
  case 2:
  case 3:
    R2C_COLOR(threshold, argv[0]);
    
    if(argc >= 2)
      cmp = R2C_ENUM(argv[1], rb_CmpOp);
    else
      cmp = ippCmpLess;

    if(argc >= 3)
      R2C_COLOR(value, argv[2]);
    else
      value = threshold;
    
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 1, 2 or 3)", argc);
    break;
  }

  newImage = image_threshold_copy(Data_Get_Struct_Ret(self, Image), &threshold, cmp, &value, &status);
  raise_on_error(status);

  return WRAP_IMAGE(newImage);
  return Qnil;
}