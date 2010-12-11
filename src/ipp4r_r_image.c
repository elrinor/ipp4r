#include "ipp4r.h"


// -------------------------------------------------------------------------- //
// rb_Image_alloc
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_alloc, (VALUE klass)) {
  TRACE_RETURN(image_wrap(NULL)); /* Underlying C struct will be allocated later, in "initialize" method */
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_initialize
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_initialize, (int argc, VALUE *argv, VALUE self)) {
  int width;
  int height;
  Image* image;
  int status;
  int border;
  VALUE filler;
  IppMetaType metaType;

  filler = Qnil;
  metaType = ipp8u_C3;
  border = 1;

  switch (argc) {
  case 5:
    filler = argv[4];
  case 4:
    border = R2C_INT(argv[3]);
  case 3:
    metaType = R2C_ENUM(argv[2], rb_MetaType);
  case 2:
    width = R2C_INT(argv[0]);
    height = R2C_INT(argv[1]);
    if(width <= 0 || height <= 0)
      rb_raise(rb_Exception, "wrong image size: %d x %d", width, height);
    image = image_new(width, height, metaType, border);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 2, 3, 4 or 5)", argc);
    break;
  }

  if(image == NULL)
    rb_raise(rb_eNoMemError, "failed to create new image");

  DATA_PTR(self) = image;
  image_share(image);

  if(filler != Qnil)
    rb_Image_fill_bang(self, filler);

  TRACE_RETURN(self);
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_load
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_load, (int argc, VALUE* argv, VALUE klass)) {
  Image* image;
  int status;
  int border;

  border = 1;

  switch (argc) {
  case 2:
    border = R2C_INT(argv[1]);
  case 1:
    Check_Type(argv[0], T_STRING);
    image = image_load(R2C_STR(argv[0]), border, &status);
    raise_on_error(status);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 1 or 2)", argc);
    break;
  }

  TRACE_RETURN(image_wrap(image));
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_initialize_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_initialize_copy, (VALUE self, VALUE other)) {
  Image* image;
  int status;

  image = image_clone(Data_Get_Struct_Ret(other, Image), &status);

  raise_on_error(status);

  DATA_PTR(self) = image;
  image_share(image);

  TRACE_RETURN(self);
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_jaehne
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_jaehne, (int argc, VALUE *argv, VALUE klass)) {
  VALUE r_image;
  int status;

  if(argc != 2 && argc != 3)
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 2 or 3)", argc);

  r_image = rb_Image_initialize(argc, argv, rb_Image_alloc(klass));

  rb_gc_register_address(&r_image); /* tell ruby gc this object is in use */
  status = image_jaehne(Data_Get_Struct_Ret(r_image, Image));
  rb_gc_unregister_address(&r_image);

  raise_on_error(status);

  TRACE_RETURN(r_image);
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_subimage
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_subimage, (int argc, VALUE *argv, VALUE self)) {
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

  TRACE_RETURN(image_wrap(newImage));
} TRACE_END


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
  raise_on_error(image_addranduniform(Data_Get_Struct_Ret(self, Image), R2M_NUM(lo), R2M_NUM(hi)));

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
// rb_Image_datatype
// -------------------------------------------------------------------------- //
VALUE rb_Image_datatype(VALUE self) {
  Image* image;
  VALUE result;

  Data_Get_Struct(self, Image, image);

  return C2R_ENUM(image_datatype(image), rb_DataType);
}


// -------------------------------------------------------------------------- //
// rb_Image_datatype
// -------------------------------------------------------------------------- //
VALUE rb_Image_metatype(VALUE self) {
  Image* image;
  VALUE result;

  Data_Get_Struct(self, Image, image);

  return C2R_ENUM(image_metatype(image), rb_MetaType);
}


// -------------------------------------------------------------------------- //
// rb_Image_convert
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, rb_Image_convert, (VALUE self, VALUE r_metatype)) {
  Image* result;
  int status;

  result = image_convert_copy(Data_Get_Struct_Ret(self, Image), R2C_ENUM(r_metatype, rb_MetaType), &status);
  raise_on_error(status);

  TRACE_RETURN(image_wrap(result));
} TRACE_END


// -------------------------------------------------------------------------- //
// rb_Image_ref
// -------------------------------------------------------------------------- //
VALUE rb_Image_ref(VALUE self, VALUE x, VALUE y) {
  Image* image;
  int cx, cy;

  image = Data_Get_Struct_Ret(self, Image);
  cx = R2C_INT(x);
  cy = R2C_INT(y);

  if(cx < 0 || cx >= image_width(image) || cy < 0 || cy >= image_height(image))
    rb_raise(rb_Exception, "trying to access pixel outside of image boundaries");

  return WRAP_COLORREF(colorref_new(image, self, cx, cy));
}


// -------------------------------------------------------------------------- //
// rb_Image_ref_eq
// -------------------------------------------------------------------------- //
VALUE rb_Image_ref_eq(VALUE self, VALUE x, VALUE y, VALUE color) {
  VALUE colorRef;

  colorRef = rb_Image_ref(self, x, y);
  
  rb_gc_register_address(&colorRef); /* tell ruby gc this object is in use */
  rb_ColorRef_set(colorRef, color);
  rb_gc_unregister_address(&colorRef);

  return colorRef;
}


// -------------------------------------------------------------------------- //
// rb_Image_fill_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_fill_bang(VALUE self, VALUE rb_color) {
  Color color;

  R2C_COLOR(&color, rb_color);

  raise_on_error(image_fill(Data_Get_Struct_Ret(self, Image), &color));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_fill
// -------------------------------------------------------------------------- //
VALUE rb_Image_fill(VALUE self, VALUE color) {
  VALUE r_image;

  r_image = rb_funcall(self, rb_ID_clone, 0);
  rb_gc_register_address(&r_image); /* tell ruby gc this object is in use */
  rb_Image_fill_bang(r_image, color);
  rb_gc_unregister_address(&r_image);

  return r_image;
}


// -------------------------------------------------------------------------- //
// rb_Image_transpose
// -------------------------------------------------------------------------- //
VALUE rb_Image_transpose(VALUE self) {
  Image* newImage;
  int status;

  newImage = image_transpose_copy(Data_Get_Struct_Ret(self, Image), &status);
  raise_on_error(status);

  return image_wrap(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_threshold_parseargs
// -------------------------------------------------------------------------- //
static void rb_Image_threshold_parseargs(int argc, VALUE* argv, Color* threshold, IppCmpOp* cmp, Color* value) {
  switch(argc) {
  case 1:
  case 2:
  case 3:
    R2C_COLOR(threshold, argv[0]);

    if(argc >= 2)
      *cmp = R2C_ENUM(argv[1], rb_CmpOp);
    else
      *cmp = ippCmpLess;

    if(argc >= 3)
      R2C_COLOR(value, argv[2]);
    else
      *value = *threshold;
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d instead of 1, 2 or 3)", argc);
    break;
  }
}


// -------------------------------------------------------------------------- //
// rb_Image_threshold
// -------------------------------------------------------------------------- //
VALUE rb_Image_threshold(int argc, VALUE* argv, VALUE self) {
  Image* newImage;
  int status;
  IppCmpOp cmp;
  Color threshold, value;
 
  rb_Image_threshold_parseargs(argc, argv, &threshold, &cmp, &value);
 
  newImage = image_threshold_copy(Data_Get_Struct_Ret(self, Image), &threshold, cmp, &value, &status);
  raise_on_error(status);

  return image_wrap(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_threshold_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_threshold_bang(int argc, VALUE* argv, VALUE self) {
  IppCmpOp cmp;
  Color threshold, value;

  rb_Image_threshold_parseargs(argc, argv, &threshold, &cmp, &value);

  raise_on_error(image_threshold(Data_Get_Struct_Ret(self, Image), &threshold, cmp, &value));

  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_dilate3x3
// -------------------------------------------------------------------------- //
VALUE rb_Image_dilate3x3(VALUE self) {
  Image* newImage;
  int status;

  newImage = image_dilate3x3_copy(Data_Get_Struct_Ret(self, Image), &status);
  raise_on_error(status);

  return image_wrap(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_dilate3x3_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_dilate3x3_bang(VALUE self) {
  raise_on_error(image_dilate3x3(Data_Get_Struct_Ret(self, Image)));
  return self;
}


// -------------------------------------------------------------------------- //
// rb_Image_erode3x3
// -------------------------------------------------------------------------- //
VALUE rb_Image_erode3x3(VALUE self) {
  Image* newImage;
  int status;

  newImage = image_erode3x3_copy(Data_Get_Struct_Ret(self, Image), &status);
  raise_on_error(status);

  return image_wrap(newImage);
}


// -------------------------------------------------------------------------- //
// rb_Image_erode3x3_bang
// -------------------------------------------------------------------------- //
VALUE rb_Image_erode3x3_bang(VALUE self) {
  raise_on_error(image_erode3x3(Data_Get_Struct_Ret(self, Image)));
  return self;
}

