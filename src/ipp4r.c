#include <ruby.h>
#include <ipp.h>
#include "ippimage.h"

static VALUE rb_mIpp;

static VALUE rb_cIppImage;
static VALUE rb_eIppException;

static void raise_on_error(IppStatus status) {
  if(status < 0) /* negative value means error */
    rb_raise(rb_eIppException, "%s", ipp_error_message(status));
}

static void rb_ipp_image_mark(IppImage* image) {}

static void rb_ipp_image_free(IppImage* image) {
  ippimage_destroy(image);
}

static VALUE rb_ipp_image_allocate(VALUE klass) {
  IppImage* image = ippimage_new();
  return Data_Wrap_Struct(klass, rb_ipp_image_mark, rb_ipp_image_free, image);
}

static VALUE rb_ipp_image_load(VALUE self, VALUE fileName) {
  IppImage* image;
  Check_Type(fileName, T_STRING);
  Data_Get_Struct(self, IppImage, image);
  raise_on_error(ippimage_load(image, RSTRING(fileName)->ptr));
  return Qnil;
}

static VALUE rb_ipp_image_save(VALUE self, VALUE fileName) {
  IppImage* image;
  Check_Type(fileName, T_STRING);
  Data_Get_Struct(self, IppImage, image);
  raise_on_error(ippimage_save(image, RSTRING(fileName)->ptr));
  return Qnil;
}

static VALUE rb_ipp_image_addranduniform(VALUE self, VALUE hi, VALUE lo) {
  IppImage* image;
  Data_Get_Struct(self, IppImage, image);
  raise_on_error(ippimage_addranduniform(image, (Ipp8u) NUM2INT(lo), (Ipp8u) NUM2INT(hi) ));
  return Qnil;
}

void Init_ipp4r() {
  rb_mIpp = rb_define_module("Ipp");

  rb_cIppImage = rb_define_class_under(rb_mIpp, "Image", rb_cObject);
  rb_define_alloc_func(rb_cIppImage, rb_ipp_image_allocate);
  rb_define_method(rb_cIppImage, "load", rb_ipp_image_load, 1);
  rb_define_method(rb_cIppImage, "save", rb_ipp_image_save, 1);
  rb_define_method(rb_cIppImage, "addRandUniform", rb_ipp_image_addranduniform, 2);

  rb_eIppException = rb_define_class_under(rb_mIpp, "Exception", rb_eStandardError);

}

