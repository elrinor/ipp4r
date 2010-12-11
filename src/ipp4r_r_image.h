#ifndef __IPP4R_R_IMAGE_H__
#define __IPP4R_R_IMAGE_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Alloc function for Image class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Image_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#initialize(width, height, [fill])</tt>
 * <li> <tt>Ipp::Image#initialize(fileName)</tt>
 * </ul>
 *         
 * Initializes a new Image object.
 * @see image_load
 * @see image_new
 */
VALUE rb_Image_initialize(int argc, VALUE *argv, VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#save(fileName)</tt>
 * </ul>
 *         
 * Saves an image to a file.
 * @see image_save
 */
VALUE rb_Image_save(VALUE self, VALUE fileName);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#add_rand_uniform!(lo, hi)</tt>
 * </ul>
 * 
 * Generates random samples with uniform distribution and adds them an input image.
 * @returns self
 * @see image_addranduniform
 * @see rb_Image_add_rand_uniform
 */
VALUE rb_Image_add_rand_uniform_bang(VALUE self, VALUE lo, VALUE hi);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#add_rand_uniform(lo, hi)</tt>
 * </ul>
 *         
 * Generates random samples with uniform distribution and adds them to a copy of input image.
 * @returns noised copy of input image
 * @see image_addranduniform
 * @see rb_Image_add_rand_uniform_bang
 */
VALUE rb_Image_add_rand_uniform(VALUE self, VALUE lo, VALUE hi);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#width</tt>
 * </ul>
 * 
 * Width reader.
 * @see image_width
 */
VALUE rb_Image_width(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#height</tt>
 * </ul>
 * 
 * Height reader.
 * @see image_height
 */
VALUE rb_Image_height(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#channels</tt>
 * </ul>
 * 
 * Channels reader.
 */
VALUE rb_Image_channels(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#convert(Channels to) </tt>
 * </ul>
 * 
 * Creates a copy of image and converts it to another channel type.
 */
VALUE rb_Image_convert(VALUE self, VALUE r_channels);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#convert!(Channels to) </tt>
 * </ul>
 * 
 * Converts given image to another channel type.
 * @returns self
 */
VALUE rb_Image_convert_bang(VALUE self, VALUE r_channels);


#ifdef __cplusplus
}
#endif

#endif