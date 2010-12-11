#include <ipp.h>
#include <ruby.h>
#include <highgui.h> /* REMOVE ME */
#include <time.h> /* for rand seed */
#include <stdio.h> /* for debug purposes */
#include "ipp4r.h"

#ifdef IS_ERROR
#  undef IS_ERROR
#endif

#define IS_ERROR(x) ((x) < 0)

// -------------------------------------------------------------------------- //
// Supplementary functions
// -------------------------------------------------------------------------- //
/**
 * Creates an IppiSize of an image
 */
static IppiSize image_ippisize(Image* image) {
  IppiSize result;
  result.height = image->height;
  result.width = image->width;
  return result;
}


// -------------------------------------------------------------------------- //
// image_new
// -------------------------------------------------------------------------- //
Image* image_new(int width, int height, IppChannels channels) {
  Image* image;
  Data* data;
  
  image = malloc(sizeof(Image)); // TODO: ALLOC

  if(image != NULL) {
    data = data_new(width, height, channels);
    if(data != NULL) {
      image->data = Data_Wrap_Struct(rb_Data, NULL, data_destroy, data);
      image->height = data->height;
      image->width = data->width;
      image->wStep = data->wStep;
      image->pixels = data->pixels;
    } else {
      free(image);
      image = NULL;
    }
  }
  return image;
}


// -------------------------------------------------------------------------- //
// image_destroy
// -------------------------------------------------------------------------- //
void image_destroy(Image* image) {
  free(image);
}


// -------------------------------------------------------------------------- //
// image_mark
// -------------------------------------------------------------------------- //
void image_mark(Image* image) {
  rb_gc_mark(image->data);
}


// -------------------------------------------------------------------------- //
// image_load
// -------------------------------------------------------------------------- //
Image* image_load(const char* fileName, IppStatus* pStatus) {
  IplImage* iplImage;
  Image* image;
  IppStatus status;

  iplImage = cvLoadImage(fileName, CV_LOAD_IMAGE_COLOR);
  if(iplImage == NULL) {
    status = ippStsErr; /* seem to be the only reasonable error value */
    goto end;
  }

  image = image_new(iplImage->width, iplImage->height, ippC3);
  if(image == NULL) {
    status = ippStsNoMemErr;
    goto error;
  }

  status = ippiCopy_8u_C3R(iplImage->imageData, iplImage->widthStep, image->pixels, image->wStep, image_ippisize(image)); // TODO: C1, AC4

error:
  cvReleaseImage(&iplImage);

end:
  if(pStatus != NULL)
    *pStatus = status;
  return image;
}


// -------------------------------------------------------------------------- //
// image_save
// -------------------------------------------------------------------------- //
IppStatus image_save(Image* image, const char* fileName) {
  /* this function is really ugly... but it's slated for removal anyway %) */
  IplImage* iplImage;
  IppStatus status;
  int result;

  iplImage = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 3);
  if(iplImage == NULL)
    return ippStsNoMemErr;

  status = ippiCopy_8u_C3R(image->pixels, image->wStep, iplImage->imageData, iplImage->widthStep, image_ippisize(image)); // TODO: C1, AC4
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
// image_addranduniform
// -------------------------------------------------------------------------- //
IppStatus image_addranduniform(Image* image, Ipp8u lo, Ipp8u hi) {
  unsigned int seed;
  static unsigned int seedMod = 0;

  seed = (unsigned int) time(NULL) + seedMod++;

  return ippiAddRandUniform_Direct_8u_C3IR(image->pixels, image->wStep, image_ippisize(image), lo, hi, &seed); // TODO: C1, AC4
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