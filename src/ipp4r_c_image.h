#ifndef __IPP4R_C_IMAGE_H__
#define __IPP4R_C_IMAGE_H__

#include <ruby.h>
#include <ippdefs.h>
#include <ippi.h>
#include "ipp4r_fwd.h"
#include "ipp4r_metatype.h"

#ifdef __cplusplus
extern "C" {
#endif

// -------------------------------------------------------------------------- //
// Typedefs
// -------------------------------------------------------------------------- //
/**
 * Main Image structure. <p>
 *
 * Note that we are using ruby gc facilities for sharing one Data structure between several Images. <br>
 * Of course this isn't the only possible solution for our problem. Other approaches are as follows:
 * <ul>
 * <li> Use internal reference counting
 *   <ul>
 *   <li> Use thread-safe reference-counting
 *   <li> Give up thread-safe counting, use <tt>++</tt> and <tt>--</tt> and hope nobody'd notice
 *   </ul>
 * <li> Don't use shared data structure
 * </ul>
 * The last approach isn't flexible enough, unsafe counting is, huh, unsafe, so the only reasonable alternative is to use thread-safe counting.
 * But implementing thread-safe counting is a real pain in the ass - we'd have to implement it several times for different platforms, operating systems, and even compilers.
 * Only on Windows we have neat InterlockedIncrement and InterlockedDecrement intrinsics. For *nix we'll have no choice but to mess with inline assembler or pthreads. <br>
 * So that's it - the simplest way is to use ruby gc. <p>
 *
 * Also note that even though we are storing a reference to Data structure, it is still considered a part of Image's internal state, 
 * i.e. <tt>clone</tt> and <tt>dup</tt> methods copy the Data.
 * The motivation here is simple: Image class is opaque and user knows nothing about it's internal structure. From outside our Image structure is perceived as a
 * "smart reference" to image data, and therefore the data itself is a part of Image's internal state. <p>
 *
 * Another thing you must know is that there exist no way to change the size of an existing Data structure. Motivation: change of size involves creating of a new image and
 * is not an in-place operation. Also change of size may invalidate pixel references or even Image structures. That's why all in-place operations does not change the size of
 * an image.<p>
 *
 * As I've noted above, we use ruby gc for sharing one Data structure between several images. It's a nice approach, but it gives rise to many subtle problems.
 * Consider the following example:
 * <code><pre>
 *   Image* img = image_new(...);
 *   // work with heap using ruby's xalloc
 *   image_transpose(img); // use image
 * </pre></code>
 * It seems everything is OK, but it's not. A call to xalloc may trigger gc, which will sweep out the underlying shared Data structure and we'll end up with a segmentation fault.
 * To avoid it, we use lazy gc registration of Data structure - it gets registered in image_wrap. And if you call image_destroy before a call to image_wrap, the Data will be 
 * deallocated.
 */
struct _Image {
  VALUE rb_data;        /**< Ruby wrapper around Data associated with this Image */
  Data* data;           /**< Pointer to Data structure associated with this Image */

  int is_subimage;      /**< is this image a subimage? i.e. should we use ROI information? */
  int x;                /**< ROI x coordinate */
  int y;                /**< ROI y coordinate */
  int width;            /**< width of ROI in pixels */
  int height;           /**< height of ROI in pixels */
};

// -------------------------------------------------------------------------- //
// Function declarations
// -------------------------------------------------------------------------- //
/**
 * Allocates memory for image.
 * 
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_new(Image** dst, int width, int height, IppMetaType metaType, int border);


/**
 * Creates a subimage, that references a part of given image
 *
 * @param image source image
 * @param dst destination image
 * @param x subimage upper-left corner x coordinate
 * @param y subimage upper-left corner y coordinate
 * @param width subimage width
 * @param height subimage width
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_subimage(Image* image, Image** dst, int x, int y, int width, int height);


/**
 * Frees the memory occupied by the input image.
 *
 * @param image image to free
 */
void image_destroy(Image* image);


/**
 * Mark function for ruby mark'n'sweep garbage collector.
 */
void image_mark(Image* image);


/**
 * Wrap function - wraps an Image* into VALUE and registers it in ruby gc.
 */
VALUE image_wrap(Image* image);


/**
 * Turns Image* into shared object by registering the underlying Data structure in ruby gc.
 */
void image_share(Image* image);


/**
 * Clones the given image (including the Data it references)
 *
 * @param image image to clone
 * @param dst destination image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_clone(Image* image, Image** dst);


/**
 * @returns height of the given image in pixels
 */
int image_height(Image* image);


/**
 * @returns width of the given image in pixels
 */
int image_width(Image* image);


/**
 * @returns channel type of a given image
 */
IppChannels image_channels(Image* image);


/**
 * @returns data type of a given image
 */
IppDataType image_datatype(Image* image);


/**
 * @returns metatype of a given image
 */
IppMetaType image_metatype(Image* image);


/**
 * @returns available image border
 */
int image_border_available(Image* image);


/**
 * Adds border pixels to an image. If a border of necessary size is already available, does nothing.
 * 
 * @param image source image
 * @param border required width of the border in pixels
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_ensure_border(Image* image, int border);


/**
 * Recalculates border pixels of an image.
 *
 * @param image source image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_rebuild_border(Image* image);


/**
 * Loads an image from a file.
 *
 * @param filename name of a file to load image from
 * @param dst destination image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_load(Image** dst, const char* filename, int border);


/**
 * Saves an image to a file.
 *
 * @param image source image
 * @param filename name of a file to save image to
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_save(Image* image, const char* filename);


/**
 * Generates random samples with uniform distribution and adds them to an input image.
 *
 * @param image source image
 * @param lo lower bound for the range of uniformly distributed values
 * @param hi upper bound for the range of uniformly distributed values
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_addranduniform(Image* image, IppMetaNumber lo, IppMetaNumber hi);


/**
 * Converts the given image to given data type. If no conversion is needed, just returns a clone of the given image (underlying data is cloned too!).
 * 
 * @param image source image
 * @param dst destination image
 * @param dataType new data type
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_convert_datatype_copy(Image* image, Image** dst, IppDataType dataType);


/**
 * Converts the given image to given channels type. If no conversion is needed, just returns a clone of the given image (underlying data is cloned too!).
 * 
 * @param image source image
 * @param dst destination image
 * @param channels new channels type
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_convert_channels_copy(Image* image, Image** dst, IppChannels channels);


/**
 * Converts the given image to given metatype. If no conversion is needed, just returns a clone of the given image (underlying data is cloned too!).
 * 
 * @param image source image
 * @param dst destination image
 * @param metaType new metatype
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_convert_copy(Image* image, Image** dst, IppMetaType metaType);


/**
 * Gets color of a pixel at (x, y)
 * 
 * @param image source image
 * @param x pixel x coordinate
 * @param y pixel y coordinate
 * @param color (OUT) color of a pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_get_pixel(Image* image, int x, int y, Color* color);


/**
 * Gets color of a pixel at (x, y)
 * 
 * @param image source image
 * @param x pixel x coordinate
 * @param y pixel y coordinate
 * @param color new color value of a pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_set_pixel(Image* image, int x, int y, Color* color);


/**
 * Fills the given image with the given color
 *
 * @param image source image
 * @param color color to use
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_fill(Image* image, Color* color);


/**
 * Transposes a source image
 *
 * @param image source image
 * @param dst destination image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_transpose_copy(Image* image, Image** dst);


/**
 * Performs in-place thresholding of pixel values in an image.
 * Pixels that satisfy the cmp relation with threshold, are set to a specified value.
 * Note that alpha channel is not thresholded.
 *
 * @param image source image
 * @param threshold threshold value
 * @param cmp relation, only ippCmpLess and ippCmpGreater are supported
 * @param value color value to set thresholded pixels to
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_threshold(Image* image, Color* threshold, IppCmpOp cmp, Color* value);


/**
 * Performs thresholding of pixel values in an image.
 * Pixels that satisfy the cmp relation with threshold, are set to a specified value.
 * Note that alpha channel is not thresholded.
 *
 * @param image source image
 * @param dst destination image
 * @param threshold threshold value
 * @param cmp relation, only ippCmpLess and ippCmpGreater are supported
 * @param value color value to set thresholded pixels to
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_threshold_copy(Image* image, Image** dst, Color* threshold, IppCmpOp cmp, Color* value);


/**
 * Creates Jaehne test image.
 *
 * @param image target image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_jaehne(Image* image);


/**
 * Creates a test image that has an intensity ramp.
 *
 * @param image target image
 * @param offset offset value
 * @param slope slope coefficient
 * @param axis specifies the direction of the image intensity ramp
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_ramp(Image* image, float offset, float slope, IppiAxis axis);


/**
 * Performs in-place dilation of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 *
 * @param image source image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_dilate3x3(Image* image);


/**
 * Performs dilation of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 *
 * @param image source image
 * @param dst destination image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_dilate3x3_copy(Image* image, Image** dst);


/**
 * Performs in-place erosion of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 *
 * @param image source image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_erode3x3(Image* image);


/**
 * Performs erosion of an image using a 3x3 mask. In the four-channel image the alpha channel is not processed.
 *
 * @param image source image
 * @param dst destination image
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_erode3x3_copy(Image* image, Image** dst);


/**
 * Blurs an image using a simple box filter.
 * 
 * @param image source image
 * @param maskSize size of the mask in pixels
 * @param anchor anchor cell specifying the mask alignment with respect to the position of the input pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_filter_box(Image* image, IppiSize maskSize, IppiPoint anchor);


/**
 * Blurs an image using a simple box filter.
 *
 * @param image source image
 * @param dst destination image
 * @param maskSize size of the mask in pixels
 * @param anchor anchor cell specifying the mask alignment with respect to the position of the input pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_filter_box_copy(Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor);


/**
 * Applies the ‘min’ filter to an image.
 *
 * @param image source image
 * @param dst destination image
 * @param maskSize size of the mask in pixels
 * @param anchor anchor cell specifying the mask alignment with respect to the position of the input pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_filter_min_copy(Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor);


/**
 * Applies the ‘max’ filter to an image.
 *
 * @param image source image
 * @param dst destination image
 * @param maskSize size of the mask in pixels
 * @param anchor anchor cell specifying the mask alignment with respect to the position of the input pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_filter_max_copy(Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor);


/**
 * Filters an image using a median filter.
 * Underlying routines doesn't support ipp32f type, therefore conversion to ipp16u is performed if the image of ipp32f datatype is passed.
 *
 * @param image source image
 * @param dst destination image
 * @param maskSize size of the mask in pixels
 * @param anchor anchor cell specifying the mask alignment with respect to the position of the input pixel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_filter_median_copy(Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor);


/**
 * Auxiliary function, returns textual description of an image_* error or warning. Don't try to deallocate the return value!
 * 
 * @param status error/warning value to get textual description for
 * @returns textual description of an error or warning
 */
const char* image_error_message(int status);


#ifdef __cplusplus
}
#endif

#endif