#include <ipp.h>
#include <ruby.h>
#include <highgui.h> /* REMOVE ME */
#include <time.h> /* for rand seed */
#include <stdio.h> /* for debug purposes */
#include "ipp4r.h"
#include "ipp4r_metatype.h"

// -------------------------------------------------------------------------- //
// Image accessors
// -------------------------------------------------------------------------- //
#define IS_SUBIMAGE(IMAGE) ((IMAGE)->is_subimage)
#define HEIGHT(IMAGE) ((IS_SUBIMAGE(IMAGE) ? (IMAGE)->height : (IMAGE)->data->height))
#define WIDTH(IMAGE) ((IS_SUBIMAGE(IMAGE) ? (IMAGE)->width : (IMAGE)->data->width))
#define PIXELSIZE(IMAGE) ((IMAGE)->data->pixelSize)
#define PIXELS(IMAGE) (image_pixels(IMAGE))
#define PIXEL_AT(IMAGE, X, Y) ((char*) PIXELS(IMAGE) + Y * WSTEP(IMAGE) + X * PIXELSIZE(IMAGE))
#define WSTEP(IMAGE) ((IMAGE)->data->wStep)
#define IPPISIZE(IMAGE) (image_ippisize(IMAGE))

#define METATYPE(IMAGE) ((IMAGE)->data->metaType)
#define CHANNELS(IMAGE) (metatype_channels(METATYPE(IMAGE)))
#define DATATYPE(IMAGE) (metatype_datatype(METATYPE(IMAGE)))


// -------------------------------------------------------------------------- //
// Supplementary functions
// -------------------------------------------------------------------------- //
/**
 * @returns IppiSize of an Image
 */
static IppiSize image_ippisize(Image* image) {
  IppiSize result;

  assert(image != NULL);

  result.height = HEIGHT(image);
  result.width = WIDTH(image);
  return result;
}


/**
 * @returns pixels pointer of an image
 */
static void* image_pixels(Image* image) {
  assert(image != NULL);

  if(IS_SUBIMAGE(image))
    return (char*) image->data->pixels + image->y * WSTEP(image) + image->x * PIXELSIZE(image);
  else
    return image->data->pixels;
}


// -------------------------------------------------------------------------- //
// image_height
// -------------------------------------------------------------------------- //
int image_height(Image* image) {
  assert(image != NULL);

  return HEIGHT(image);
}


// -------------------------------------------------------------------------- //
// image_width
// -------------------------------------------------------------------------- //
int image_width(Image* image) {
  assert(image != NULL);

  return WIDTH(image);
}


// -------------------------------------------------------------------------- //
// image_channels
// -------------------------------------------------------------------------- //
IppChannels image_channels(Image* image) {
  assert(image != NULL);

  return CHANNELS(image);
}


// -------------------------------------------------------------------------- //
// image_datatype
// -------------------------------------------------------------------------- //
IppDataType image_datatype(Image* image) {
  assert(image != NULL);

  return DATATYPE(image);
}


// -------------------------------------------------------------------------- //
// image_metatype
// -------------------------------------------------------------------------- //
IppMetaType image_metatype(Image* image) {
  assert(image != NULL);

  return METATYPE(image);
}


// -------------------------------------------------------------------------- //
// image_new
// -------------------------------------------------------------------------- //
Image* image_new(int width, int height, IppMetaType metaType) {
  Image* image;
  Data* data;

  assert(width > 0 && height > 0);

  image = ALLOC(Image); // ALLOC always succeeds or throws an exception
  data = data_new(width, height, metaType);

  if(data != NULL) {
    image->data = data;
    image->rb_data = Data_Wrap_Struct(rb_Data, NULL, data_destroy, data);
    image->is_subimage = FALSE;
  } else {
    xfree(image);
    image = NULL;
  }

  return image;
}


// -------------------------------------------------------------------------- //
// image_subimage
// -------------------------------------------------------------------------- //
Image* image_subimage(Image* image, int x, int y, int width, int height, int* pStatus) {
  Image* newImage;
  
  assert(image != NULL);
  assert(x >= 0 && y >= 0 && x + width <= WIDTH(image) && y + height <= HEIGHT(image));

  newImage = ALLOC(Image); // ALLOC always succeeds or throws an exception

  newImage->data = image->data; // data is shared
  newImage->rb_data = image->rb_data;
  newImage->is_subimage = TRUE;
  newImage->x = x;
  newImage->y = y;
  newImage->width = width;
  newImage->height = height;

  if(pStatus != NULL)
    *pStatus = ippStsNoErr;
  return newImage;
}


// -------------------------------------------------------------------------- //
// image_destroy
// -------------------------------------------------------------------------- //
void image_destroy(Image* image) {
  assert(image != NULL);

  xfree(image);
}


// -------------------------------------------------------------------------- //
// image_mark
// -------------------------------------------------------------------------- //
void image_mark(Image* image) {
  /* I check for NULL not because of paranoia.
   * The problem is that in rb_Image_alloc we initialize image with NULL, and subsequent call to rb_Image_initialize may trigger gc while our image is still NULL! */
  if(image != NULL)
    rb_gc_mark(image->rb_data);
}


// -------------------------------------------------------------------------- //
// image_clone
// -------------------------------------------------------------------------- //
Image* image_clone(Image* image, int* pStatus) {
  Image* newImage;
  int status;

  assert(image != NULL);

  newImage = image_new(WIDTH(image), HEIGHT(image), METATYPE(image));

  if(newImage != NULL) {
    IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiCopy_, R, (PIXELS(image), WSTEP(image), PIXELS(newImage), WSTEP(newImage), IPPISIZE(image))), Unreachable(), ippStsBadArgErr);
    if(IS_ERROR(status)) {
      image_destroy(newImage);
      newImage = NULL;
    }
  }

  if(pStatus != NULL)
    *pStatus = status;
  return newImage;
}


// -------------------------------------------------------------------------- //
// image_load
// -------------------------------------------------------------------------- //
Image* image_load(const char* fileName, int* pStatus) {
  IplImage* iplImage;
  Image* image;
  int status;

  assert(fileName != NULL);

  iplImage = cvLoadImage(fileName, CV_LOAD_IMAGE_COLOR);
  if(iplImage == NULL) {
    status = ippStsErr; /* seem to be the only reasonable error value */
    goto end;
  }

  image = image_new(iplImage->width, iplImage->height, ipp8u_C3); // always succeeds or throws
  if(image == NULL) {
    status = ippStsNoMemErr;
    goto error;
  }

  status = ippiCopy_8u_C3R(iplImage->imageData, iplImage->widthStep, PIXELS(image), WSTEP(image), IPPISIZE(image));

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
int image_save(Image* image, const char* fileName) {
  IplImage* iplImage;
  int status;
  int result;

  assert(image != NULL && fileName != NULL);

  iplImage = cvCreateImage(cvSize(WIDTH(image), HEIGHT(image)), IPL_DEPTH_8U, 3);
  if(iplImage == NULL)
    return ippStsNoMemErr;

  /* TODO: rewrite with convert! */
  switch(CHANNELS(image)) {
  case ippC1:
    status = ippiDup_8u_C1C3R(PIXELS(image), WSTEP(image), iplImage->imageData, iplImage->widthStep, IPPISIZE(image));
  	break;
  case ippC3:
    status = ippiCopy_8u_C3R(PIXELS(image), WSTEP(image), iplImage->imageData, iplImage->widthStep, IPPISIZE(image));
    break;
  case ippAC4:
    status = ippiCopy_8u_AC4C3R(PIXELS(image), WSTEP(image), iplImage->imageData, iplImage->widthStep, IPPISIZE(image));
    break;
  default:
    status = ippStsBadArgErr;
    Unreachable();
  }

  if(IS_ERROR(status))
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
int image_addranduniform(Image* image, IppMetaNumber lo, IppMetaNumber hi) {
  unsigned int seed;
  int status;
  static unsigned int seedMod = 0;

  assert(image != NULL);

  seed = (unsigned int) time(NULL) + seedMod++;

#define METAFUNC(M, ARG) ARX_JOIN_3(ippiAddRandUniform_Direct_, M, IR) (PIXELS(image), WSTEP(image), IPPISIZE(image), M2C_NUMBER(M, lo), M2C_NUMBER(M, hi), &seed)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC  
  return status;
}


// -------------------------------------------------------------------------- //
// image_convert_copy
// -------------------------------------------------------------------------- //
Image* image_convert_copy(Image* image, IppChannels channels, int* pStatus) {
  int status;
  Image *result, *result2;
  int specialCase;

  /* TODO */

  assert(image != NULL);

  if(CHANNELS(image) == channels) {
    /* nothing to convert */
    *pStatus = ippStsNoErr;
    return image_clone(image, pStatus);
  }

  if(CHANNELS(image) == ippC1 && channels == ippAC4) {
    /* needs special handling */
    channels = ippC3;
    specialCase = TRUE;
  } else
    specialCase = FALSE;

  result = image_new(WIDTH(image), HEIGHT(image), channels);

  switch(CHANNELS(image)) {
    case ippC1:
      switch(channels) {
        case ippC3:
          status = ippiDup_8u_C1C3R(PIXELS(image), WSTEP(image), PIXELS(result), WSTEP(result), IPPISIZE(image));
          break;
        default:
          status = ippStsBadArgErr;
          Unreachable();
      }
      break;
    case ippC3:
      switch(channels) {
        case ippC1:
          status = ippiRGBToGray_8u_C3C1R(PIXELS(image), WSTEP(image), PIXELS(result), WSTEP(result), IPPISIZE(image));
          break;
        case ippAC4:
          status = ippiCopy_8u_C3AC4R(PIXELS(image), WSTEP(image), PIXELS(result), WSTEP(result), IPPISIZE(image));
          break;
        default:
          status = ippStsBadArgErr;
          Unreachable();
      }
      break;
    case ippC4:
      switch(channels) {
        case ippC1:
          status = ippiRGBToGray_8u_AC4C1R(PIXELS(image), WSTEP(image), PIXELS(result), WSTEP(result), IPPISIZE(image));
          break;
        case ippC3:
          status = ippiCopy_8u_AC4C3R(PIXELS(image), WSTEP(image), PIXELS(result), WSTEP(result), IPPISIZE(image));
          break;
        default:
          status = ippStsBadArgErr;
          Unreachable();
      }
      break;
    default:
      status = ippStsBadArgErr;
      Unreachable();
  }

  if(IS_ERROR(status)) {
    image_destroy(result);
    result = NULL;
  } else if(specialCase) {
    result2 = image_convert_copy(result, ippAC4, &status); // TODO: rewrite it
    image_destroy(result);
    result = result2;
  }

  if(pStatus != NULL)
    *pStatus = status;
  return result;
}


// -------------------------------------------------------------------------- //
// image_convert
// -------------------------------------------------------------------------- //
int image_convert(Image* image, IppChannels channels) {
  int status;
  Image* newImage;

  if(CHANNELS(image) == channels)
    return ippStsNoErr;

  newImage = image_convert_copy(image, channels, &status);

  if(IS_ERROR(status))
    return status;

  data_swap(image->data, newImage->data); // neat cheat! %)
  image_destroy(newImage);

  return ippStsNoErr;  
}


// -------------------------------------------------------------------------- //
// image_get_pixel
// -------------------------------------------------------------------------- //
int image_get_pixel(Image* image, int x, int y, Color* color) {
  char* p;

  assert(image != NULL);
  assert(x >= 0 && y >= 0 && x < WIDTH(image) && y <= HEIGHT(image));

  p = PIXEL_AT(image, x, y);

  color->a = 1.0f;
  switch(CHANNELS(image)) {
  case ippC1:
    color->r = color->g = color->b = *p;
  	break;
  case ippAC4:
    color->as_array[3] = *(p + 3);
  case ippC3:
    color->as_array[2] = *(p + 2);
    color->as_array[1] = *(p + 1);
    color->as_array[0] = *(p + 0);
    break;
  default:
    Unreachable();
    return ippStsBadArgErr;
  }

  return ippStsNoErr;
}


// -------------------------------------------------------------------------- //
// image_set_pixel
// -------------------------------------------------------------------------- //
int image_set_pixel(Image* image, int x, int y, Color* color) {
  char* p;

  assert(image != NULL);
  assert(x >= 0 && y >= 0 && x < WIDTH(image) && y <= HEIGHT(image));

  p = PIXEL_AT(image, x, y);

  M2C_COLOR_TO(8u_C3, color->as_array, p);

  M2C_COLOR_TO(8u_C1, color->as_array, p);

  switch(CHANNELS(image)) {
  case ippC1:
    *p = (unsigned char) color_gray(color);
    break;
  case ippAC4:
    *(p + 3) = (unsigned char) color->as_array[3];
  case ippC3:
    *(p + 2) = (unsigned char) color->as_array[2];
    *(p + 1) = (unsigned char) color->as_array[1];
    *(p + 0) = (unsigned char) color->as_array[0];
    break;
  default:
    Unreachable();
    return ippStsBadArgErr;
  }

  return ippStsNoErr;
}


// -------------------------------------------------------------------------- //
// image_fill
// -------------------------------------------------------------------------- //
int image_fill(Image* image, Color* color) {
  int status;
  USING_M2C_COLOR(1);

  assert(image != NULL && color != NULL);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiSet_, M, R) (M2C_COLOR(M, color->as_array, 0), PIXELS(image), WSTEP(image), IPPISIZE(image))
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  return status;
}


// -------------------------------------------------------------------------- //
// image_transpose_copy
// -------------------------------------------------------------------------- //
Image* image_transpose_copy(Image* image, int* pStatus) {
  Image* newImage;
  int status;

  assert(image != NULL);

  newImage = image_new(HEIGHT(image), WIDTH(image), CHANNELS(image));
  if(newImage == NULL) {
    status = ippStsNoMemErr;
    goto end;
  }

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiTranspose_, M_REPLACE_C_IF_C(M_REPLACE_D_IF_D(M, 32f, 32s), AC4, C4), R) /* TODO: test this hack*/ \
  (PIXELS(image), WSTEP(image), PIXELS(newImage), WSTEP(newImage), IPPISIZE(image))
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  if(IS_ERROR(status)) {
    image_destroy(newImage);
    newImage = NULL;
  }

end:
  if(pStatus != NULL)
    *pStatus = status;
  return newImage;
}


// -------------------------------------------------------------------------- //
// image_threshold_copy
// -------------------------------------------------------------------------- //
Image* image_threshold_copy(Image* image, Color* threshold, IppCmpOp cmp, Color* value, int* pStatus) {
  Image* newImage;
  int status;
  USING_M2C_COLOR(2);

  assert(image != NULL && threshold != NULL && value != NULL);
  assert(cmp == ippCmpLess || cmp == ippCmpGreater);

  newImage = image_new(WIDTH(image), HEIGHT(image), CHANNELS(image));
  if(newImage == NULL) {
    status = ippStsNoMemErr;
    goto end;
  }

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiThreshold_Val_, M, R)                  \
  (PIXELS(image), WSTEP(image), PIXELS(newImage), WSTEP(newImage), IPPISIZE(image), M2C_COLOR(M, threshold->as_array, 0), M2C_COLOR(M, value->as_array, 1), cmp)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

end:
  if(pStatus != NULL)
    *pStatus = status;
  return newImage;
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
// image_error_message
// -------------------------------------------------------------------------- //
const char* image_error_message(int status) {
  return ippGetStatusString(status);
}