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
#define PIXEL_AT(IMAGE, X, Y) ((void*)((char*) PIXELS(IMAGE) + Y * WSTEP(IMAGE) + X * PIXELSIZE(IMAGE)))
#define WSTEP(IMAGE) ((IMAGE)->data->wStep)
#define IPPISIZE(IMAGE) (image_ippisize(IMAGE))
#define SHARED(IMAGE) ((IMAGE)->data->shared)
#define BORDER(IMAGE) ((IMAGE)->data->border)
#define BORDER_AVAILABLE(IMAGE) (image_border_available(IMAGE))

#define METATYPE(IMAGE) ((IMAGE)->data->metaType)
#define CHANNELS(IMAGE) (metatype_channels(METATYPE(IMAGE)))
#define DATATYPE(IMAGE) (metatype_datatype(METATYPE(IMAGE)))

#define CHECK_DATATYPE_IN_DA(IMAGE, LEFT_PART, DA) IPPMETACALL(DATATYPE(IMAGE), LEFT_PART, DA, IPPMETAVAL, TRUE, ARX_EMPTY(), FALSE)
#define CHECK_CHANNELS_IN_CA(IMAGE, LEFT_PART, CA) IPPMETACALL(CHANNELS(IMAGE), LEFT_PART, CA, IPPMETAVAL, TRUE, ARX_EMPTY(), FALSE)

// -------------------------------------------------------------------------- //
// Helpers
// -------------------------------------------------------------------------- //
#define PWPWI(src, dst) PIXELS(src), WSTEP(src), PIXELS(dst), WSTEP(dst), IPPISIZE(src) /* IPPISIZE must be for src! see ippiman for Transpose function */
#define PWI(src) PIXELS(src), WSTEP(src), IPPISIZE(src)


// -------------------------------------------------------------------------- //
// Supplementary functions
// -------------------------------------------------------------------------- //
/**
 * @returns IppiSize of an Image
 */
static IppiSize image_ippisize(Image* image) {
  return ippi_size(WIDTH(image), HEIGHT(image));
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

/**
 * @returns required border size for filter with mask of size maskSize and anchor point specified by anchor.
 */
static int required_border(IppiSize maskSize, IppiPoint anchor) {
  return max(max(anchor.x, anchor.y), max(maskSize.width - anchor.x - 1, maskSize.height - anchor.y - 1));
}

/**
 * @returns required border size for given IppiMaskSize
 */
static int masksize_border(IppiMaskSize maskSize) {
  switch(maskSize) {
  case ippMskSize1x3: return 1;
  case ippMskSize1x5: return 2;
  case ippMskSize3x1: return 1;
  case ippMskSize3x3: return 1;
  case ippMskSize5x1: return 2;
  case ippMskSize5x5: return 2;
  default: 
    Unreachable();
    return 0;
  }
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
// image_border_available
// -------------------------------------------------------------------------- //
int image_border_available(Image* image) {
  assert(image != NULL);

  if(!IS_SUBIMAGE(image))
    return BORDER(image);
  else {
    int borderU = BORDER(image) + image->y;
    int borderD = BORDER(image) + image->data->height - (image->y + image->height);
    int borderL = BORDER(image) + image->x;
    int borderR = BORDER(image) + image->data->width - (image->x + image->width);
    return min(min(borderU, borderD), min(borderL, borderR));
  }
}


// -------------------------------------------------------------------------- //
// image_ensure_border
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_ensure_border, (Image* image, int border)) {
  Image* result;
  int status;
  int borderAvailable;
  int borderGrowth;
  IppiSize dstRoi, srcRoi;

  assert(image != NULL);
  assert(border >= 0);

  borderAvailable = BORDER_AVAILABLE(image);
  if(borderAvailable >= border)
    TRACE_RETURN(ippStsNoErr);

  borderGrowth = border - borderAvailable;
  
  if(IS_ERROR(status = image_new(&result, image->data->width, image->data->height, METATYPE(image), BORDER(image) + borderGrowth)))
    TRACE_RETURN(status);

  srcRoi.width  = image->data->width   + 2 * BORDER(image);
  srcRoi.height = image->data->height  + 2 * BORDER(image);
  dstRoi.width  = result->data->width  + 2 * BORDER(result);
  dstRoi.height = result->data->height + 2 * BORDER(result);
                                                                /* TODO: test this hack */
#define METAFUNC(M, ARG) ARX_JOIN_3(ippiCopyReplicateBorder_, M_REPLACE_D_IF_D(M, 32f, 32s), R) (image->data->buffer, WSTEP(image), srcRoi, result->data->buffer, WSTEP(result), dstRoi, borderGrowth, borderGrowth)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC  
  if(IS_ERROR(status)) {
    image_destroy(result);
    TRACE_RETURN(status);
  }

  data_swap(image->data, result->data);
  image_destroy(result);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_rebuild_border
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_rebuild_border, (Image* image)) {
  int status;
  IppiSize dstRoi, srcRoi;

  assert(image != NULL);

  srcRoi.width  = image->data->width;
  srcRoi.height = image->data->height;
  dstRoi.width  = image->data->width  + 2 * BORDER(image);
  dstRoi.height = image->data->height + 2 * BORDER(image);

#define METAFUNC(M, ARG) ARX_JOIN_3(ippiCopyReplicateBorder_, M_REPLACE_D_IF_D(M, 32f, 32s), IR) (image->data->pixels, WSTEP(image), srcRoi, dstRoi, BORDER(image), BORDER(image))
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC  

  TRACE_RETURN(status);
} TRACE_END


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
TRACE_FUNC(int, image_new, (Image** dst, int width, int height, IppMetaType metaType, int border)) {
  Data* data;

  assert(dst != NULL);
  assert(width > 0 && height > 0 && border >= 0);

  /* See motivation for malloc usage in the source for data_new() function */
  *dst = (Image*) malloc(sizeof(Image));
  if(*dst == NULL)
    TRACE_RETURN(ippStsNoMemErr);

  data = data_new(width, height, metaType, border);
  if(data == NULL) {
    free(*dst);
    TRACE_RETURN(ippStsNoMemErr);
  }

  (*dst)->data = data;
  (*dst)->is_subimage = FALSE;

  TRACE_RETURN(ippStsNoErr);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_subimage
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_subimage, (Image* image, Image** dst, int x, int y, int width, int height)) {
  assert(image != NULL && dst != NULL);
  assert(SHARED(image));
  assert(x >= 0 && y >= 0 && x + width <= WIDTH(image) && y + height <= HEIGHT(image));

  *dst = (Image*) malloc(sizeof(Image));
  if(*dst == NULL)
    TRACE_RETURN(ippStsNoMemErr);

  (*dst)->data = image->data; // data is shared
  (*dst)->rb_data = image->rb_data;
  (*dst)->is_subimage = TRUE;
  (*dst)->x = x;
  (*dst)->y = y;
  (*dst)->width = width;
  (*dst)->height = height;

  TRACE_RETURN(ippStsNoErr);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_destroy
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, image_destroy, (Image* image)) {
  assert(image != NULL);

  if(!SHARED(image))
    data_destroy(image->data);
  free(image);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_mark
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, image_mark, (Image* image)) {
  TRACE(("%08X", image));
  /* I check for NULL not because of paranoia.
   * The problem is that in rb_Image_alloc we initialize image with NULL, and subsequent call to rb_Image_initialize may trigger gc while our image is still NULL! */
  if(image != NULL) {
    assert(SHARED(image));
    TRACE(("data=%08X", image->data));
    rb_gc_mark(image->rb_data);
  }
} TRACE_END


// -------------------------------------------------------------------------- //
// image_share
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, image_share, (Image* image)) {
  assert(image != NULL);
  assert(!SHARED(image));

  TRACE(("%08X data=%08X", image, image->data));

  image->data->shared = TRUE;
  image->rb_data = Data_Wrap_Struct(rb_Data, NULL, data_destroy, image->data);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_wrap
// -------------------------------------------------------------------------- //
TRACE_FUNC(VALUE, image_wrap, (Image* image)) {
  /* Check for NULL is needed, see rb_Image_alloc.
   * Check for SHARED is needed too, see rb_Image_subimage */
  if(image != NULL && !SHARED(image)) 
    image_share(image);
  TRACE_RETURN(Data_Wrap_Struct(rb_Image, image_mark, image_destroy, image));
} TRACE_END


// -------------------------------------------------------------------------- //
// image_clone
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_clone, (Image* image, Image** dst)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiCopy_, R, (PWPWI(image, *dst))), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_load
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_load, (Image** dst, const char* fileName, int border)) {
  IplImage* iplImage;
  int status;

  assert(dst != NULL);
  assert(fileName != NULL && border >= 0);

  iplImage = cvLoadImage(fileName, CV_LOAD_IMAGE_COLOR);
  if(iplImage == NULL)
    TRACE_RETURN(ippStsErr); /* seem to be the only reasonable error value */

  if(IS_ERROR(status = image_new(dst, iplImage->width, iplImage->height, ipp8u_C3, border)))
    goto error;

  if(IS_ERROR(status = ippiCopy_8u_C3R(iplImage->imageData, iplImage->widthStep, PWI(*dst)))) {
    image_destroy(*dst);
    goto error;
  }

  if(IS_ERROR(status = image_rebuild_border(*dst)))
    image_destroy(*dst);

error:
  cvReleaseImage(&iplImage);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_save
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_save, (Image* image, const char* fileName)) {
  IplImage* iplImage;
  Image* converted;
  int status;
  int result;

  assert(image != NULL && fileName != NULL);

  if(METATYPE(image) != ipp8u_C3) {
    if(IS_ERROR(status = image_convert_copy(image, &converted, ipp8u_C3)))
      TRACE_RETURN(status);
  } else
    converted = image;

  iplImage = cvCreateImage(cvSize(WIDTH(image), HEIGHT(image)), IPL_DEPTH_8U, 3);
  if(iplImage == NULL) {
    status = ippStsNoMemErr;
    goto end;
  }

  status = ippiCopy_8u_C3R(PIXELS(converted), WSTEP(converted), iplImage->imageData, iplImage->widthStep, IPPISIZE(converted));
  if(IS_ERROR(status))
    goto error;
  
  result = cvSaveImage(fileName, iplImage); /* cvSaveImage returns zero on failure, non-zero otherwise */
  if(result == 0)
    status = ippStsErr; /* seem to be the only reasonable error value */

error:
  cvReleaseImage(&iplImage);

end:
  if(METATYPE(image) != ipp8u_C3)
    image_destroy(converted);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_addranduniform
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_addranduniform, (Image* image, IppMetaNumber lo, IppMetaNumber hi)) {
  unsigned int seed;
  int status;
  static unsigned int seedMod = 0;

  assert(image != NULL);

  seed = (unsigned int) time(NULL) + seedMod++;

#define METAFUNC(M, ARG) ARX_JOIN_3(ippiAddRandUniform_Direct_, M, IR) (PWI(image), M2C_NUMBER(M, lo), M2C_NUMBER(M, hi), &seed)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC  
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_convert_datatype_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_convert_datatype_copy, (Image* image, Image** dst, IppDataType dataType)) {
  int status;
  Ipp32f scaleColor[4] = {D_SCALE(16u), D_SCALE(16u), D_SCALE(16u), D_SCALE(16u)};

  assert(image != NULL && dst != NULL);

  TRACE(("image_convert_datatype from %d to %d", DATATYPE(image), dataType));

  if(DATATYPE(image) == dataType)
    TRACE_RETURN(image_clone(image, dst));

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), metatype_compose(dataType, CHANNELS(image)), 0)))
    TRACE_RETURN(status);

  TRACE(("image_convert_datatype from %d to %d", DATATYPE(image), dataType));

#define CONVERT(D, C, NEW_D)                                                    \
  IF_D_EQ_D(D, NEW_D,                                                           \
    Unreachable(),                                                              \
    ARX_IF(DD_IN_DDA((D, NEW_D), (2, ((16u, 8u), (8u, 16u)))),                  \
      status = ARX_JOIN_6(ippiScale_, D, NEW_D, _, C, R) (PWPWI(image, *dst) ARX_COMMA_IF(D_EQ_D(D, 16u)) ARX_IF(D_EQ_D(D, 16u), ippAlgHintNone, ARX_EMPTY())), \
      ARX_IF(DD_IN_DDA((D, NEW_D), (2, ((32f, 8u), (8u, 32f)))),                \
        status = ARX_JOIN_6(ippiScale_, D, NEW_D, _, C, R) (PWPWI(image, *dst), 0.0f, 1.0f), \
        IF_DD_EQ_DD((D, NEW_D), (16u, 32f),                                     \
          status = ARX_JOIN_6(ippiConvert_, D, NEW_D, _, C, R) (PWPWI(image, *dst)); \
          if(!IS_ERROR(status))                                                 \
            status = ARX_JOIN_5(ippiDivC_, NEW_D, _, IF_C_EQ_C(C, AC4, C4, C), IR) (scaleColor IF_C_EQ_C(C, C1, [0], ARX_EMPTY()), PWI(*dst)), \
          IF_DD_EQ_DD((D, NEW_D), (32f, 16u),                                   \
            {                                                                   \
              Image* newImage;                                                  \
              if(!IS_ERROR(status = image_new(&newImage, WIDTH(image), HEIGHT(image), METATYPE(image), 0))) { \
                status = ARX_JOIN_5(ippiMulC_, D, _, IF_C_EQ_C(C, AC4, C4, C), R) (PIXELS(image), WSTEP(image), scaleColor IF_C_EQ_C(C, C1, [0], ARX_EMPTY()), PWI(newImage)); \
                if(!IS_ERROR(status))                                           \
                  status = ARX_JOIN_6(ippiConvert_, D, NEW_D, _, C, R) (PWPWI(newImage, *dst), ippRndZero); \
                image_destroy(newImage);                                        \
              }                                                                 \
            },                                                                  \
            OMG_TEH_DRAMA                                                       \
          )                                                                     \
        )                                                                       \
      )                                                                         \
    )                                                                           \
  )
#define METAFUNC_2(NEW_D, M) CONVERT(M_DATATYPE(M), M_CHANNELS(M), NEW_D)
#define METAFUNC(M, ARGS) IPPMETACALL(dataType, ARX_EMPTY(), D_SUPPORTED, METAFUNC_2, M, Unreachable(); status = ippStsBadArgErr, ARX_EMPTY())
  IPPMETACALL(METATYPE(image), ARX_EMPTY(), M_SUPPORTED, METAFUNC, ~, Unreachable(); status = ippStsBadArgErr, ARX_EMPTY());
#undef METAFUNC
#undef METAFUNC_2
#undef CONVERT

  TRACE(("end_convert"));

  if(IS_ERROR(status))
    image_destroy(*dst);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_convert_channels_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_convert_channels_copy, (Image* image, Image** dst, IppChannels channels)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(CHANNELS(image) == channels)
    TRACE_RETURN(image_clone(image, dst));

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), metatype_compose(DATATYPE(image), channels), 0)))
    TRACE_RETURN(status);

  #define CONVERT(D, C, NEW_C)                                                  \
  IF_C_EQ_C(C, NEW_C,                                                           \
    Unreachable(),                                                              \
    ARX_IF(CC_IN_CCA((C, NEW_C), (2, ((AC4, C3), (C3, AC4)))),                  \
      status = ARX_JOIN_6(ippiCopy_, D, _, C, NEW_C, R) (PWPWI(image, *dst)); \
      IF_CC_EQ_CC((C, NEW_C), (C3, AC4),                                        \
        if(!IS_ERROR(status))                                                   \
          status = ARX_JOIN_3(ippiSet_, D, _C4CR) (D_MAX(D), ((D_CTYPE(D)*) PIXELS(*dst)) + 3, WSTEP(*dst), IPPISIZE(*dst)), \
        ARX_EMPTY()                                                             \
      ),                                                                        \
      ARX_IF(CC_IN_CCA((C, NEW_C), (2, ((C3, C1), (AC4, C1)))),                 \
        status = ARX_JOIN_6(ippiRGBToGray_, D, _, C, NEW_C, R) (PWPWI(image, *dst)), \
        ARX_IF(ARX_AND(D_EQ_D(D, 8u), CC_EQ_CC((C, NEW_C), (C1, C3))),          \
          status = ARX_JOIN_6(ippiDup_, D, _, C, NEW_C, R) (PWPWI(image, *dst)), \
          ARX_IF(CC_IN_CCA((C, NEW_C), (2, ((C1, C3), (C1, AC4)))),             \
            {                                                                   \
              D_CTYPE(D)* pSrc[4];                                              \
              pSrc[0] = pSrc[1] = pSrc[2] = pSrc[3] = (D_CTYPE(D)*) PIXELS(image); \
              status = ARX_JOIN_7(ippiCopy_, D, _, P, C_CNUMB(NEW_C), IF_C_EQ_C(NEW_C, AC4, C4, NEW_C), R) (pSrc, WSTEP(image), PIXELS(*dst), WSTEP(*dst), IPPISIZE(*dst)); \
              IF_C_EQ_C(NEW_C, AC4,                                             \
                if(!IS_ERROR(status))                                           \
                  status = ARX_JOIN_3(ippiSet_, D, _C4CR) (D_MAX(D), ((D_CTYPE(D)*) PIXELS(*dst)) + 3, WSTEP(*dst), IPPISIZE(*dst));, \
                ARX_EMPTY()                                                     \
              )                                                                 \
            },                                                                  \
            OMG_TEH_DRAMA                                                       \
          )                                                                     \
        )                                                                       \
      )                                                                         \
    )                                                                           \
  )
#define METAFUNC_2(NEW_C, M) CONVERT(M_DATATYPE(M), M_CHANNELS(M), NEW_C)
#define METAFUNC(M, ARGS) IPPMETACALL(channels, ARX_EMPTY(), C_SUPPORTED, METAFUNC_2, M, Unreachable(); status = ippStsBadArgErr, ARX_EMPTY())
  IPPMETACALL(METATYPE(image), ARX_EMPTY(), M_SUPPORTED, METAFUNC, ~, Unreachable(); status = ippStsBadArgErr, ARX_EMPTY());
#undef METAFUNC
#undef METAFUNC_2
#undef CONVERT

  if(IS_ERROR(status))
    image_destroy(*dst);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_convert_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_convert_copy, (Image* image, Image** dst, IppMetaType metaType)) {
  int status;
  Image *tmp;
  IppChannels channels;
  IppDataType dataType;

  assert(image != NULL && dst != NULL);
  assert(is_metatype_supported(metaType));

  TRACE(("begin_convert from %d to %d", METATYPE(image), metaType));

  channels = metatype_channels(metaType);
  dataType = metatype_datatype(metaType);

  if(CHANNELS(image) != channels) {
    if(IS_ERROR(status = image_convert_channels_copy(image, &tmp, channels)))
      TRACE_RETURN(status);
    
    if(DATATYPE(image) != dataType) {
      status = image_convert_datatype_copy(tmp, dst, dataType);
      image_destroy(tmp);
    } else
      *dst = tmp;
  } else
    TRACE_RETURN(image_convert_datatype_copy(image, dst, dataType));

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_get_pixel
// -------------------------------------------------------------------------- //
int image_get_pixel(Image* image, int x, int y, Color* color) {
  void* p;

  assert(image != NULL);
  assert(x >= 0 && y >= 0 && x < WIDTH(image) && y <= HEIGHT(image));

  p = PIXEL_AT(image, x, y);

#define METAFUNC(M, ARGS) C2M_COLOR_TO(M, p, color->as_array)
  IPPMETACALL(METATYPE(image), ARX_EMPTY(), M_SUPPORTED, METAFUNC, ~, Unreachable(); return ippStsBadArgErr, ARX_EMPTY())
#undef METAFUNC

  return ippStsNoErr;
}


// -------------------------------------------------------------------------- //
// image_set_pixel
// -------------------------------------------------------------------------- //
int image_set_pixel(Image* image, int x, int y, Color* color) {
  void* p;

  assert(image != NULL);
  assert(x >= 0 && y >= 0 && x < WIDTH(image) && y <= HEIGHT(image));

  p = PIXEL_AT(image, x, y);

#define METAFUNC(M, ARGS) M2C_COLOR_TO(M, color->as_array, p)
  IPPMETACALL(METATYPE(image), ARX_EMPTY(), M_SUPPORTED, METAFUNC, ~, Unreachable(); return ippStsBadArgErr, ARX_EMPTY())
#undef METAFUNC

  return ippStsNoErr;
}


// -------------------------------------------------------------------------- //
// image_fill
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_fill, (Image* image, Color* color)) {
  int status;
  USING_M2C_COLOR(1);

  assert(image != NULL && color != NULL);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiSet_, M, R) (M2C_COLOR(M, color->as_array, 0), PWI(image))
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_transpose_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_transpose_copy, (Image* image, Image** dst)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_new(dst, HEIGHT(image), WIDTH(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiTranspose_, M_REPLACE_C_IF_C(M_REPLACE_D_IF_D(M, 32f, 32s), AC4, C4), R) (PWPWI(image, *dst)) /* 32f -> 32s hack works, tested */
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_threshold
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_threshold, (Image* image, Color* threshold, IppCmpOp cmp, Color* value)) {
  int status;
  USING_M2C_COLOR(2);

  assert(image != NULL && threshold != NULL && value != NULL);
  assert(cmp == ippCmpLess || cmp == ippCmpGreater);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiThreshold_Val_, M, IR) (PWI(image), M2C_COLOR(M, threshold->as_array, 0), M2C_COLOR(M, value->as_array, 1), cmp)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_threshold_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_threshold_copy, (Image* image, Image** dst, Color* threshold, IppCmpOp cmp, Color* value)) {
  int status;
  USING_M2C_COLOR(2);

  assert(image != NULL && dst != NULL && threshold != NULL && value != NULL);
  //assert(cmp == ippCmpLess || cmp == ippCmpGreater); TODO: do we need this?

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiThreshold_Val_, M, R) (PWPWI(image, *dst), M2C_COLOR(M, threshold->as_array, 0), M2C_COLOR(M, value->as_array, 1), cmp)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_jaehne 
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_jaehne, (Image* image)) {
  int status;

  assert(image != NULL);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiImageJaehne_, R, (PWI(image))), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_ramp
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_ramp, (Image* image, float offset, float slope, IppiAxis axis)) {
  int status;

  assert(image != NULL);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiImageRamp_, R, (PWI(image), offset, slope, axis)), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_dilate3x3
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_dilate3x3, (Image* image)) {
  int status;

  assert(image != NULL);

  if(IS_ERROR(status = image_ensure_border(image, 1)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiDilate3x3_, IR, (PWI(image))), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_dilate3x3_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_dilate3x3_copy, (Image* image, Image** dst)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, 1)))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiDilate3x3_, R, (PWPWI(image, *dst))), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_erode3x3
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_erode3x3, (Image* image)) {
  int status;

  assert(image != NULL);

  if(IS_ERROR(status = image_ensure_border(image, 1)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiErode3x3_, IR, (PWI(image))), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_erode3x3_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_erode3x3_copy, (Image* image, Image** dst)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, 1)))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiErode3x3_, R, (PWPWI(image, *dst))), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_dilate
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_dilate, (Image* image, Matrix* mask, IppiPoint anchor)) {
  int status;
  IppiSize maskSize;

  assert(image != NULL && mask != NULL);
  assert(mask->isMask);

  if(IS_ERROR(status = image_ensure_border(image, required_border(mask->size, anchor))))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiDilate_, IR, (PWI(image), (char*) mask->data, mask->size, anchor)), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_dilate_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_dilate_copy, (Image* image, Image** dst, Matrix* mask, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL && mask != NULL);
  assert(mask->isMask);

  if(IS_ERROR(status = image_ensure_border(image, required_border(mask->size, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiDilate_, R, (PWPWI(image, *dst), (char*) mask->data, mask->size, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_erode
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_erode, (Image* image, Matrix* mask, IppiPoint anchor)) {
  int status;
  IppiSize maskSize;

  assert(image != NULL && mask != NULL);
  assert(mask->isMask);

  if(IS_ERROR(status = image_ensure_border(image, required_border(mask->size, anchor))))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiErode_, IR, (PWI(image), (char*) mask->data, mask->size, anchor)), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_erode_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_erode_copy, (Image* image, Image** dst, Matrix* mask, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL && mask != NULL);
  assert(mask->isMask);

  if(IS_ERROR(status = image_ensure_border(image, required_border(mask->size, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiErode_, R, (PWPWI(image, *dst), (char*) mask->data, mask->size, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_box
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_box, (Image* image, IppiSize maskSize, IppiPoint anchor)) {
  int status;

  assert(image != NULL);

  if(IS_ERROR(status = image_ensure_border(image, required_border(maskSize, anchor))))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiFilterBox_, IR, (PWI(image), maskSize, anchor)), Unreachable(), ippStsBadArgErr);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_box_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_box_copy, (Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, required_border(maskSize, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiFilterBox_, R, (PWPWI(image, *dst), maskSize, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_min_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_min_copy, (Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, required_border(maskSize, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiFilterMin_, R, (PWPWI(image, *dst), maskSize, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_max_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_max_copy, (Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, required_border(maskSize, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiFilterMax_, R, (PWPWI(image, *dst), maskSize, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_median_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_median_copy, (Image* image, Image** dst, IppiSize maskSize, IppiPoint anchor)) {
  int status;
  int created = FALSE;

  assert(image != NULL && dst != NULL);

  CHECK_DATATYPE_IN_DA(image, status =, (3, (8u, 16u, 16s)));
  if(!status) {
    image_convert_datatype_copy(image, &image, ipp16u);
    created = TRUE;
  }

  if(IS_ERROR(status = image_ensure_border(image, required_border(maskSize, anchor)))) {
    if(created)
      image_destroy(image);
    TRACE_RETURN(status);
  }

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0))) {
    if(created)
      image_destroy(image);
    TRACE_RETURN(status);
  }

                                          /* TODO: use macro like MA_INTERSECT_DA */
  IPPMETACALL(METATYPE(image), status =, (6,  (8u_C1, 8u_C3, 8u_AC4, 16u_C1, 16u_C3, 16u_AC4)), IPPMETAFUNC, (ippiFilterMedian_, R, (PWPWI(image, *dst), maskSize, anchor)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  if(created)
    image_destroy(image);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_gauss_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_gauss_copy, (Image* image, Image** dst, IppiMaskSize maskSize)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_ensure_border(image, masksize_border(maskSize))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiFilterGauss_, R, (PWPWI(image, *dst), maskSize)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_filter_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_filter_copy, (Image* image, Image** dst, Matrix* kernel, IppiPoint anchor)) {
  int status;

  assert(image != NULL && dst != NULL && kernel != NULL);
  assert(!kernel->isMask);

  if(IS_ERROR(status = image_ensure_border(image, required_border(kernel->size, anchor))))
    TRACE_RETURN(status);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

#define METAFUNC(M, ARGS) ARX_JOIN_5(IF_M_IS_D(M, 32f, ippiFilter_, ippiFilter32f_), M_DATATYPE(M), _, M_CHANNELS(M), R) (PWPWI(image, *dst), (Ipp32f*) kernel->data, kernel->size, anchor)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_draw
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_draw, (Image* image, Image* src, IppiPoint pos)) {
  int status;
  Image* source;
  IppiSize roi;
  IppiPoint shrink;

  assert(image != NULL && src != NULL);

  if(-pos.x >= WIDTH(src) || -pos.y >= HEIGHT(src) || pos.x >= WIDTH(image) || pos.y >= HEIGHT(image))
    TRACE_RETURN(ippStsNoErr);

  shrink = ippi_point(pos.x < 0 ? -pos.x : 0, pos.y < 0 ? -pos.y : 0);
  pos = ippi_point(max(0, pos.x), max(0, pos.y));
  roi = ippi_size(min(WIDTH(src) - shrink.x, WIDTH(image) - pos.x), min(HEIGHT(src) - shrink.y, HEIGHT(image) - pos.y));

  if(METATYPE(image) == METATYPE(src))
    source = src;
  else if(IS_ERROR(status = image_convert_copy(src, &source, METATYPE(image))))
    TRACE_RETURN(status);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiCopy_, R, (PIXEL_AT(source, shrink.x, shrink.y), WSTEP(source), PIXEL_AT(image, pos.x, pos.y), WSTEP(image), roi)), Unreachable(), ippStsBadArgErr);

  if(source != src)
    image_destroy(source);
  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_draw_rotated
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_draw_rotated, (Image* image, Image* src, double angle, double xShift, double yShift)) {
  int status;
  Image* source;
  IppiRect srcRoi, dstRoi;

  assert(image != NULL && src != NULL);

  if(METATYPE(image) == METATYPE(src))
    source = src;
  else if(IS_ERROR(status = image_convert_copy(src, &source, METATYPE(image))))
    TRACE_RETURN(status);

  srcRoi.x = srcRoi.y = 0;
  srcRoi.height = HEIGHT(source);
  srcRoi.width = WIDTH(source);

  dstRoi.x = dstRoi.y = 0;
  dstRoi.height = HEIGHT(image);
  dstRoi.width = WIDTH(image);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiRotate_, R, (PIXELS(source), IPPISIZE(source), WSTEP(source), srcRoi, PIXELS(image), WSTEP(image), dstRoi, angle, xShift, yShift, IPPI_INTER_CUBIC | IPPI_SMOOTH_EDGE)), Unreachable(), ippStsBadArgErr);
  
  if(source != src)
    image_destroy(source);
  TRACE_RETURN(status);
} TRACE_END



// -------------------------------------------------------------------------- //
// image_resize_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_resize_copy, (Image* image, Image** dst, IppiSize newSize)) {
  int status;
  IppiRect srcRoi;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_new(dst, newSize.width, newSize.height, METATYPE(image), 0)))
    TRACE_RETURN(status);

  srcRoi.x = srcRoi.y = 0;
  srcRoi.height = HEIGHT(image);
  srcRoi.width = WIDTH(image);

  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, IPPMETAFUNC, (ippiResize_, R, (PIXELS(image), IPPISIZE(image), WSTEP(image), srcRoi, PWI(*dst), (double) WIDTH(*dst) / WIDTH(image), (double) HEIGHT(*dst) / HEIGHT(image), IPPI_INTER_CUBIC)), Unreachable(), ippStsBadArgErr);
  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_mirror
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_mirror, (Image* image, IppiAxis axis)) {
  int status;

  assert(image != NULL);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiMirror_, M_REPLACE_D_IF_D(M, 32f, 32s), IR) (PWI(image), axis)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_mirror_copy
// -------------------------------------------------------------------------- //
TRACE_FUNC(int, image_mirror_copy, (Image* image, Image** dst, IppiAxis axis)) {
  int status;

  assert(image != NULL && dst != NULL);

  if(IS_ERROR(status = image_new(dst, WIDTH(image), HEIGHT(image), METATYPE(image), 0)))
    TRACE_RETURN(status);

#define METAFUNC(M, ARGS) ARX_JOIN_3(ippiMirror_, M_REPLACE_D_IF_D(M, 32f, 32s), R) (PWPWI(image, *dst), axis)
  IPPMETACALL(METATYPE(image), status =, M_SUPPORTED, METAFUNC, ~, Unreachable(), ippStsBadArgErr);
#undef METAFUNC

  if(IS_ERROR(status))
    image_destroy(*dst);

  TRACE_RETURN(status);
} TRACE_END


// -------------------------------------------------------------------------- //
// image_error_message
// -------------------------------------------------------------------------- //
const char* image_error_message(int status) {
  return ippGetStatusString(status);
}

