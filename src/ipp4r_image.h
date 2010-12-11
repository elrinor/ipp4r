#ifndef __IPP4R_IMAGE_H__
#define __IPP4R_IMAGE_H__

#include <ruby.h>
#include <ippdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Main Image structure.
 */
typedef struct _Image {
  VALUE data;           /**< Ruby wrapper around Data associated with this Image */

  void* pixels;         /**< ROI pointer to aligned image data */
  int width;            /**< width of ROI in pixels */
  int height;           /**< height of ROI in pixels */
  int wStep;            /**< size of aligned image row in bytes */
} Image;


/**
 * Allocates memory for the image and initializes its fields with zeros, so that no accidental misuse is possible.
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
 * Loads an image from a file.
 *
 * @param filename name of a file to load image from
 * @param status (OUT) pointer to a variable to write an error/warning code or ippStsNoErr to
 * @returns a newly loaded image, or NULL in case of an error
 */
Image* image_load(const char* filename, IppStatus* pStatus);


/**
 * Saves an image to a file.
 *
 * @param image source image
 * @param filename name of a file to save image to
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
IppStatus image_save(Image* image, const char* filename);


/**
 * Generates random samples with uniform distribution and adds them to an input image.
 *
 * @param image source image
 * @param lo lower bound for the range of uniformly distributed values
 * @param hi upper bound for the range of uniformly distributed values
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
IppStatus image_addranduniform(Image* image, Ipp8u lo, Ipp8u hi);


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
 * Auxiliary function, returns textual description of an ipp error or warning. Don't try to deallocate the return value!
 * 
 * @param status IppStatus to get textual description for
 * @returns textual description of an ipp error or warning
 */
const char* ipp_error_message(IppStatus status);

#ifdef __cplusplus
}
#endif

#endif