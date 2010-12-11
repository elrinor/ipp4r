#include <ruby.h>
#include <ipp.h>

#define IPP4R_MAIN
#include "ipp4r.h"


// TODO: other compilers...
#if defined(_MSC_VER) || defined(__ICL)
#  pragma comment(lib, "ippi.lib")
#  pragma comment(lib, "ippcore.lib")
#  pragma comment(lib, "highgui.lib") /* REMOVE ME */
#  pragma comment(lib, "cxcore.lib") /* REMOVE ME */
#endif



// -------------------------------------------------------------------------- //
// Supplementary functions...
// -------------------------------------------------------------------------- //
static void raise_on_error(IppStatus status) {
  if(status < 0) /* negative value means error */
    rb_raise(rb_Exception, "%s", ipp_error_message(status));
}


// -------------------------------------------------------------------------- //
// Image class implementation
// -------------------------------------------------------------------------- //
/**
 * Alloc function for Image class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Image_alloc(VALUE klass) {
  VALUE image;
  image = Data_Wrap_Struct(klass, NULL, image_destroy, NULL); /* Underlying C struct will be allocated later, in "initialize" method */
  return image;
}


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image.initialize(width, height, [fill])</tt>
 * <li> <tt>Ipp::Image.initialize(fileName)</tt>
 * </ul>
 *         
 * Initializes a new Image object.
 */
VALUE rb_Image_initialize(int argc, VALUE *argv, VALUE self) {
  int width;
  int height;
  Image* image;
  IppStatus status;

  switch (argc) {
  case 2:
    width = NUM2INT(argv[0]);
    height = NUM2INT(argv[1]);
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
    rb_raise(rb_eNoMemError, "not enough memory");

  DATA_PTR(self) = image;

  return self;
}


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image.save(fileName)</tt>
 * </ul>
 *         
 * Saves an image to a file.
 */
VALUE rb_Image_save(VALUE self, VALUE fileName) {
  Image* image;

  Check_Type(fileName, T_STRING);
  Data_Get_Struct(self, Image, image);

  raise_on_error(image_save(image, RSTRING(fileName)->ptr));
  return Qnil;
}


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image.add_rand_uniform(hi, lo)</tt>
 * </ul>
 *         
 * Generates random samples with uniform distribution and adds them to an input image.
 * @see ippimage_addranduniform
 */
VALUE rb_Image_add_rand_uniform(VALUE self, VALUE hi, VALUE lo) {
  Image* image;
  
  Data_Get_Struct(self, Image, image);
  
  raise_on_error(image_addranduniform(image, (Ipp8u) NUM2INT(lo), (Ipp8u) NUM2INT(hi) ));
  return Qnil;
}


// -------------------------------------------------------------------------- //
// Some macro black magic...
// -------------------------------------------------------------------------- //
DEFINE_READER(Image, height, INT)
DEFINE_READER(Image, width, INT)


// -------------------------------------------------------------------------- //
// Init
// -------------------------------------------------------------------------- //
/**
 * Ruby Init routine.
 */
void Init_ipp4r() {
  rb_Ipp = rb_define_module("Ipp");

  rb_Image = rb_define_class_under(rb_Ipp, "Image", rb_cObject);

  rb_define_alloc_func(rb_Image, rb_Image_alloc);
  rb_define_method(rb_Image, "initialize", rb_Image_initialize, -1);
  rb_define_method(rb_Image, "save", rb_Image_save, 1);
  rb_define_method(rb_Image, "add_rand_uniform", rb_Image_add_rand_uniform, 2);
  RB_DEFINE_READER(Image, height);
  RB_DEFINE_READER(Image, width);

  rb_Data = rb_define_class_under(rb_Image, "Data", rb_cObject);

  rb_Exception = rb_define_class_under(rb_Ipp, "Exception", rb_eStandardError);

}


