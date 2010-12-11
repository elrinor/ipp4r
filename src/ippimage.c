#include "ippimage.h"

#include <ipp.h>
#include <highgui.h> /* REMOVE ME */

#include <time.h> /* for rand seed */

#include <stdio.h> /* for debug purposes */

// TODO: other compilers...
#if defined(_MSC_VER) || defined(__ICL)
#  pragma comment(lib, "ippi.lib")
#  pragma comment(lib, "ippcore.lib")
#  pragma comment(lib, "highgui.lib") /* REMOVE ME */
#  pragma comment(lib, "cxcore.lib") /* REMOVE ME */
#endif

#define ALLOCATED(x) ((x)->data != NULL)
#define NOTALLOCATED(x) (!ALLOCATED(x))

#define IS_ERROR(x) ((x) < 0)

// -------------------------------------------------------------------------- //
// Supplementary functions
// -------------------------------------------------------------------------- //
/**
 * Creates an IppiSize structure of an image
 */
static IppiSize ippimage_ippisize(IppImage* image) {
  IppiSize result;
  result.height = image->height;
  result.width = image->width;
  return result;
}

/**
 * Frees memory occupied by image's pixel buffer. 
 * Does nothing if the buffer is not allocated (i.e. the image is in <tt>NOTALLOCATED</tt> state)<p>
 *
 * <ul>
 * <li>Input image state: <tt>ANY</tt>
 * <li>Output image state: <tt>NOTALLOCATED</tt>
 * </ul>
 */
static void ippimage_freebuffer(IppImage* image) {
  if(ALLOCATED(image))
    ippiFree(image->data);
  image->data = NULL;
  image->width = 0;
  image->height = 0;
  image->wStep = 0;
}

/**
 * Allocates memory for pixel data of size width x height. 
 * If the memory was already allocated, i.e. the image was in <tt>ALLOCATED</tt> state, then deallocates pixel buffer first, so that no memory leak is possible. <p>
 *
 * <ul>
 * <li>Input image state: <tt>ANY</tt>
 * <li>Output image state: <tt>ALLOCATED</tt>
 * </ul>
 *
 * @returns ippStsNoErr if everything went OK, non-zero error or warning code otherwise
 */
static IppStatus ippimage_allocbuffer(IppImage* image, int width, int height) {
  int wStep;
  void* data;

  if(width <= 0 || height <= 0)
    return ippStsBadArgErr;

  data = (void*) ippiMalloc_8u_C3(width, height, &wStep);
  if(data == NULL)
    return ippStsNoMemErr;

  ippimage_freebuffer(image);

  image->data = data;
  image->height = height;
  image->width = width;
  image->wStep = wStep;

  return ippStsNoErr;
}


// -------------------------------------------------------------------------- //
// ippimage_new
// -------------------------------------------------------------------------- //
IppImage* ippimage_new() {
  IppImage* image = (IppImage*) malloc(sizeof(IppImage));
  if(image != NULL) {
    image->data = NULL;
    ippimage_freebuffer(image);
  }
  return image;
}

// -------------------------------------------------------------------------- //
// ippimage_destroy
// -------------------------------------------------------------------------- //
void ippimage_destroy(IppImage* image) {
  ippimage_freebuffer(image);
  free(image);
}

// -------------------------------------------------------------------------- //
// ippimage_load
// -------------------------------------------------------------------------- //
IppStatus ippimage_load(IppImage* image, const char* fileName) {
  IplImage* iplImage;
  IppStatus status;

  iplImage = cvLoadImage(fileName, CV_LOAD_IMAGE_COLOR);
  if(iplImage == NULL)
    return ippStsErr; /* seem to be the only reasonable error value */

  status = ippimage_allocbuffer(image, iplImage->width, iplImage->height);
  if(IS_ERROR(status))
    goto error;

  status = ippiCopy_8u_C3R(iplImage->imageData, iplImage->widthStep, image->data, image->wStep, ippimage_ippisize(image));

error:
  cvReleaseImage(&iplImage);
  return status;
}

// -------------------------------------------------------------------------- //
// ippimage_save
// -------------------------------------------------------------------------- //
IppStatus ippimage_save(IppImage* image, const char* fileName) {
  /* this function is really ugly... but it's slated for removal anyway %) */
  IplImage* iplImage;
  IppStatus status;
  int result;

  if(NOTALLOCATED(image))
    return ippStsBadArgErr;

  iplImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 3);
  if(iplImage == NULL)
    return ippStsNoMemErr;

  status = ippiCopy_8u_C3R(image->data, image->wStep, iplImage->imageData, iplImage->widthStep, ippimage_ippisize(image));
  if(IS_ERROR(ippStsNoErr))
    goto error;
  
  result = cvSaveImage(fileName, iplImage); /* cvSaveImage returns zero on failure, non-zero otherwise */
  if(result == 0)
    status = ippStsErr; /* seem to be the only reasonable error value */

error:
  cvReleaseImage(&iplImage);
  return status;
}

// -------------------------------------------------------------------------- //
// ippimage_addranduniform
// -------------------------------------------------------------------------- //
IppStatus ippimage_addranduniform(IppImage* image, Ipp8u lo, Ipp8u hi) {
  unsigned int seed;
  static unsigned int seedMod = 0;

  if(NOTALLOCATED(image))
    return ippStsBadArgErr;

  seed = (unsigned int) time(NULL) + seedMod++;

  return ippiAddRandUniform_Direct_8u_C3IR(image->data, image->wStep, ippimage_ippisize(image), lo, hi, &seed);
}

// -------------------------------------------------------------------------- //
// ippimage_gaussianblur
// -------------------------------------------------------------------------- //
/*IppStatus ippimage_gaussianblur(IppImage* src, IppImage* dst, float sigma) {
  IppStatus status;
  int bufferSize;
  int kernelSize;
  void* buffer;

  if(NOTALLOCATED(src) || sigma <= 0)
    return ippStsBadArgErr;

  kernelSize = ((int)(sigma * 4)) * 2 + 1;
  if(kernelSize < 3)
    kernelSize = 3;

  status = ippiFilterGaussGetBufferSize_32f_C1R(ippimage_ippisize(src), kernelSize, &bufferSize);
  if(IS_ERROR(status))
    return status;

  buffer = malloc(bufferSize);
  if(buffer == NULL)
    return ippStsNoMemErr;

  if(NOTALLOCATED(dst) || src->width != dst->width || src->height != dst->height) {
    ippimage_freebuffer(dst);
    status = ippimage_allocbuffer(dst, src->width, src->height);
    if(IS_ERROR(status))
      goto error;
  }

  status = ippiFilterGaussBorder_32f_C1R(src->data, src->wStep, dst->data, dst->wStep, ippimage_ippisize(src), kernelSize, sigma, ippBorderRepl, 0, buffer);

error:
  free(buffer);
  return status;
}*/

// -------------------------------------------------------------------------- //
// ipp_error_message
// -------------------------------------------------------------------------- //
const char* ipp_error_message(IppStatus status) {
  return ippGetStatusString(status);
}