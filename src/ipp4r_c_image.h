#ifndef __IPP4R_C_IMAGE_H__
#define __IPP4R_C_IMAGE_H__

#include <ruby.h>
#include <ippdefs.h>
#include "ipp4r_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Main Image structure. <p>
 *
 * Note that we are using ruby gc facilities for sharing one Data structure between several Images. <br/>
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
 * Only on Windows we have neat InterlockedIncrement and InterlockedDecrement intrinsics. For *nix we'll have no choice but to mess with inline assembler or pthreads. <br/>
 * So that's it - the simplest way is to use ruby gc. <p>
 *
 * Also note that even though we are storing a reference to Data structure, it is still considered a part of Image's internal state, 
 * i.e. <tt>clone</tt> and <tt>dup</tt> methods copy the Data.
 * The motivation here is simple: Image class is opaque and user knows nothing about it's internal structure. From outside our Image structure is perceived as a
 * "smart reference" to image data, and therefore the data itself is a part of Image's internal state.
 */
typedef struct _Image {
  VALUE rb_data;        /**< Ruby wrapper around Data associated with this Image */
  Data* data;           /**< Pointer to Data structure associated with this Image */

  int is_subimage;      /**< is this image a subimage? i.e. should we use ROI information? */
  int x;                /**< ROI x coordinate */
  int y;                /**< ROI y coordinate */
  int width;            /**< width of ROI in pixels */
  int height;           /**< height of ROI in pixels */
} Image;


/**
 * Allocates memory for image.
 * 
 * @returns newly allocated Image, or NULL in case of an error.
 */
Image* image_new(int width, int height, IppChannels channels);


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
 * Clones the given image (including the Data it references)
 *
 * @param image image to clone
 * @param pStatus (OUT) pointer to a variable to write an error/warning code or ippStsNoErr to
 * @returns a newly allocated image which is a copy of the given one, or NULL in case of an error.
 */
Image* image_clone(Image* image, int* pStatus);


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
 * Loads an image from a file.
 *
 * @param filename name of a file to load image from
 * @param pStatus (OUT) pointer to a variable to write an error/warning code or ippStsNoErr to
 * @returns a newly loaded image, or NULL in case of an error
 */
Image* image_load(const char* filename, int* pStatus);


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
int image_addranduniform(Image* image, int lo, int hi);


/**
 * Converts given image to given channel type.
 * 
 * @param image source image
 * @param channels new channel type
 * @param pStatus (OUT) pointer to a variable to write an error/warning code or ippStsNoErr to
 * @returns a newly created converted image, or NULL in case of an error
 */
Image* image_convert_copy(Image* image, IppChannels channels, int* pStatus);


/**
 * Converts given image to given channel type.
 * 
 * @param image source image
 * @param channels new channel type
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
int image_convert(Image* image, IppChannels channels);


/*
 * Applies gaussian blur filter on src image, storing result in dst.
 *
 * @param src source image
 * @param dst destination image
 * @param sigma standard deviation of the gaussian kernel
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
/*IppStatus ippimage_gaussianblur(IppImage* src, IppImage* dst, float sigma);*/


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