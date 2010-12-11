#ifndef __IPP4R_R_IMAGE_H__
#define __IPP4R_R_IMAGE_H__

#ifdef __cplusplus
extern "C" {
#endif


// -------------------------------------------------------------------------- //
// Function Declarations
// -------------------------------------------------------------------------- //
/**
 * Alloc function for Image class. Note that the memory is actually allocated in "initialize" method.
 */
VALUE rb_Image_alloc(VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#initialize(width, height, metatype = Ipp8u_C3, border = 1, [fill])</tt>
 * </ul>
 *         
 * Initializes a new Image object.
 * @see image_new
 */
VALUE rb_Image_initialize(int argc, VALUE *argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#load(filename, border = 1)</tt>
 * </ul>
 *
 * Load an image from file.
 */
VALUE rb_Image_load(int argc, VALUE* argv, VALUE self);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#initialize_copy(image)</tt>
 * </ul>
 * 
 * Initialize copy, for clone & dup
 */
VALUE rb_Image_initialize_copy(VALUE self, VALUE other);


/**
 * Singleton method:
 * <ul>
 * <li> <tt>Ipp::Image#jaehne(width, height, metatype = Ipp8u_C3)</tt>
 * </ul>
 *
 * Creates Jaehne test image
 */
VALUE rb_Image_jaehne(int argc, VALUE *argv, VALUE klass);


/**
 * Singleton method:
 * <ul>
 * <li> <tt>Ipp::Image#ramp(width, height, metatype, offset, slope, axis = AxsHorizontal)</tt>
 * </ul>
 *
 * Creates a test image that has an intensity ramp
 */
VALUE rb_Image_ramp(int argc, VALUE *argv, VALUE klass);


/**
 * Method: 
 * <ul>
 * <li> <tt>Ipp::Image#subimage(x, y, width=0, height=0)</tt>
 * </ul>
 * 
 * Returns a reference to a subimage of current image.
 * Zero or negative values for width / height indicate offset from lower-right corner of an image.
 */
VALUE rb_Image_subimage(int argc, VALUE *argv, VALUE self);


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
 * <li> <tt>Ipp::Image#datatype</tt>
 * </ul>
 * 
 * Datatype reader.
 */
VALUE rb_Image_datatype(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#metatype</tt>
 * </ul>
 * 
 * Metatype reader.
 */
VALUE rb_Image_metatype(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#convert(MetaType to) </tt>
 * </ul>
 * 
 * Creates a copy of image and converts it to another metatype. Note that new image is created even in case no conversion is needed.
 */
VALUE rb_Image_convert(VALUE self, VALUE r_channels);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#[](x, y) </tt>
 * </ul>
 * 
 * @returns a color reference to pixel at (x, y)
 */
VALUE rb_Image_ref(VALUE self, VALUE x, VALUE y);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#[]=(x, y, color) </tt>
 * </ul>
 * 
 * Sets a color of image at (x, y) to color
 * @returns color
 */
VALUE rb_Image_ref_eq(VALUE self, VALUE x, VALUE y, VALUE color);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#fill!(color) </tt>
 * </ul>
 * 
 * Fills image with the given color
 * @returns self
 */
VALUE rb_Image_fill_bang(VALUE self, VALUE rb_color);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#fill(color) </tt>
 * </ul>
 * 
 * Copies an image and fills a copy with the given color
 */
VALUE rb_Image_fill(VALUE self, VALUE color);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#transpose </tt>
 * </ul>
 * 
 * @returns a transposed copy of a source image
 */
VALUE rb_Image_transpose(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#threshold(threshold, cmp = Ipp::LessThan, value = threshold) </tt>
 * </ul>
 * 
 * @returns a thresholded copy of a source image
 */
VALUE rb_Image_threshold(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#threshold!(threshold, cmp = Ipp::LessThan, value = threshold) </tt>
 * </ul>
 * 
 * Performs thresholding of pixel values in an image
 * @returns self
 */
VALUE rb_Image_threshold_bang(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#dilate3x3 </tt>
 * </ul>
 *
 * @returns a dilated copy of a source image
 * @see rb_Image_dilate3x3_bang
 */
VALUE rb_Image_dilate3x3(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#dilate3x3! </tt>
 * </ul>
 *
 * Performs dilation of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 * @returns self
 */
VALUE rb_Image_dilate3x3_bang(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#erode3x3 </tt>
 * </ul>
 *
 * @returns an eroded copy of a source image
 * @see rb_Image_erode3x3_bang
 */
VALUE rb_Image_erode3x3(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#erode3x3! </tt>
 * </ul>
 *
 * Performs erosion of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 * @returns self
 */
VALUE rb_Image_erode3x3_bang(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_box(size, anchor = {size.width / 2, size.height / 2}) </tt>
 * </ul>
 *
 * Blurs an image using a simple box filter.
 * @returns a newly created blurred image
 */
VALUE rb_Image_filter_box(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_box!(size, anchor = {size.width / 2, size.height / 2}) </tt>
 * </ul>
 *
 * Blurs an image using a simple box filter.
 * @returns self
 */
VALUE rb_Image_filter_box_bang(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_min(size, anchor = {size.width / 2, size.height / 2}) </tt>
 * </ul>
 *
 * Applies the ‘min’ filter to an image.
 * @returns a newly created filtered image
 */
VALUE rb_Image_filter_min(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_max(size, anchor = {size.width / 2, size.height / 2}) </tt>
 * </ul>
 *
 * Applies the ‘max’ filter to an image.
 * @returns a newly created filtered image
 */
VALUE rb_Image_filter_max(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_median(size, anchor = {size.width / 2, size.height / 2}) </tt>
 * </ul>
 *
 * Filters an image using a median filter.
 * @returns a newly created filtered image
 */
VALUE rb_Image_filter_median(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#filter_gauss(MaskSize) </tt>
 * </ul>
 *
 * Filters an image using a gauss filter.
 * @returns a newly created filtered image
 */
VALUE rb_Image_filter_gauss(int argc, VALUE* argv, VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#rebuild_border!</tt>
 * </ul>
 *
 * Recalculates the border pixels of an image
 * @returns self
 */
VALUE rb_Image_rebuild_border_bang(VALUE self);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#ensure_border!(int size)</tt>
 * </ul>
 *
 * Adds border pixels to an image. If a border of necessary size is already available, does nothing.
 * @returns self
 */
VALUE rb_Image_ensure_border_bang(VALUE self, VALUE size);


/**
 * Method:
 * <ul>
 * <li> <tt>Ipp::Image#border</tt>
 * </ul>
 * 
 * @returns available image border
 */
VALUE rb_Image_border(VALUE self);



#ifdef __cplusplus
}
#endif

#endif