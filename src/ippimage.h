#ifndef __IPPIMAGE_H__
#define __IPPIMAGE_H__

#include <ippdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Main IppImage structure. <p>
 * 
 * IppImage stores data pointer, data type, and image sizes. Can be in one of the following states:
 * <ul>
 * <li><tt>NOTALLOCATED</tt> - data pointer is <tt>NULL</tt>, most operations will "safely" fail.
 * <li><tt>ALLOCATED</tt> - data pointer != <tt>NULL</tt>, ready for performing operations.
 * </ul>
 */
typedef struct _IppImage {
  void* data;   /**< pointer to aligned image data */

  int width;    /**< image width in pixels */
  int height;   /**< image height in pixels */
  int wStep;    /**< size of aligned image row in bytes */
} IppImage;

/**
 * Allocates memory for the image and initializes its fields with zeros, so that no accidental misuse is possible. <p>
 * 
 * <ul>
 * <li>Output image state: <tt>NOTALLOCATED</tt>
 * </ul>
 * 
 * @returns newly allocated IppImage, or NULL in case of an error.
 */
IppImage* ippimage_new();

/**
 * Frees the memory occupied by the input image. <p>
 * 
 * <ul>
 * <li>Input image state: <tt>ANY</tt>
 * </ul>
 *
 * @param image image to free
 */
void ippimage_destroy(IppImage* image);

/**
 * Loads an image from a file. <p>
 *
 * <ul>
 * <li>Input image state: <tt>ANY</tt>
 * <li>Output image state: <tt>ALLOCATED</tt>
 * </ul>
 *
 * @param image source image
 * @param filename name of a file to load image from
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
IppStatus ippimage_load(IppImage* image, const char* filename);

/**
 * Saves an image to a file. <p>
 *
 * <ul>
 * <li>Input image state: <tt>ALLOCATED</tt>
 * <li>Output image state: <tt>ALLOCATED</tt>
 * </ul>
 *
 * @param image source image
 * @param filename name of a file to save image to
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
IppStatus ippimage_save(IppImage* image, const char* filename);

/**
 * Generates random samples with uniform distribution and adds them to an image data. <p>
 *
 * <ul>
 * <li>Input image state: <tt>ALLOCATED</tt>
 * <li>Output image state: <tt>ALLOCATED</tt>
 * </ul>
 *
 * @param image source image
 * @param lo lower bound for the range of uniformly distributed values
 * @param hi upper bound for the range of uniformly distributed values
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
IppStatus ippimage_addranduniform(IppImage* image, Ipp8u lo, Ipp8u hi);

/*
 * Applies gaussian blur filter on src image, storing result in dst.
 *
 * <ul>
 * <li>Input src state: <tt>ALLOCATED</tt>
 * <li>Output src state: <tt>ALLOCATED</tt>
 * </ul>
 * <ul>
 * <li>Input dst state: <tt>ANY</tt>
 * <li>Output dst state: <tt>ALLOCATED</tt>
 * </ul>
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