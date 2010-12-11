#include <assert.h>
#include <ipp.h>
#include <ruby.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// Supplementary functions
// -------------------------------------------------------------------------- //
/**
 * Body block for xmalloc
 */
VALUE xmalloc_body(VALUE x) {
  return (VALUE) xmalloc((long) x);
}


/**
 * Protected version of xmalloc - does not throw, returns NULL instead.
 */
void* xmalloc_protected(long size) {
  int status;
  void* result;

  result = (void*) rb_protect(xmalloc_body, (VALUE) size, &status);
  if(status != 0) /* exception was raised*/
    return NULL;
  else
    return result;
}


/**
 * Allocates a memory block aligned to a 32-byte boundary. <br/>
 * Much like the one from ipp, but uses xmalloc provided by ruby. <br/>
 * 
 * @returns a pointer to a newly allocated block, or NULL in case of an error
 */
static void* ipp4rMalloc(int width, int height, IppMetaType metaType, int* wStep) {
  char* ptr;
  char* result;

  assert(width > 0 && height > 0);

  *wStep = (width * metatype_pixel_size(metaType) + 32 - 1) & -32;

  ptr = xmalloc_protected(*wStep * height + 32 + sizeof(char*) - 1);
  if(ptr == NULL)
    return NULL;

  result = (char*)((long)(ptr + 32 - 1) & -32);
  if(result - ptr < sizeof(char*))
    result += 32;
  *((char**)result - 1) = ptr;

  return result;
}


/**
 * Frees a memory block allocated by ipp4rMalloc
 */
static void ipp4rFree(void* ptr) {
  assert(ptr != NULL);

  xfree(*((char**)ptr - 1));
}


// -------------------------------------------------------------------------- //
// data_new
// -------------------------------------------------------------------------- //
TRACE_FUNC(Data*, data_new, (int width, int height, IppMetaType metaType)) {
  int wStep;
  Data* data;
  void* pixels;

  assert(width > 0 && height > 0);

  /* We use malloc and not ALLOC here because:
   * 1. We don't want to mess with exception handling - our function mustn't throw.
   * 2. Data structure is small, therefore ruby GC won't benefit from knowing about it. */
  data = (Data*) malloc(sizeof(Data)); 
  if(data == NULL)
    TRACE_RETURN(NULL);

  pixels = ipp4rMalloc(width, height, metaType, &wStep); 
  if(pixels == NULL) {
    free(data);
    TRACE_RETURN(NULL);
  }

  data->pixels = pixels;
  data->metaType = metaType;
  data->height = height;
  data->width = width;
  data->wStep = wStep;
  data->pixelSize = metatype_pixel_size(metaType);
  TRACE_RETURN(data);
} TRACE_END


// -------------------------------------------------------------------------- //
// data_destroy
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, data_destroy, (Data* data)) {
  assert(data != NULL);

  TRACE(("free_data %d: h=%d, w=%d, m=%d, p=%d", data, data->height, data->width, data->metaType, data->pixels))

  ipp4rFree(data->pixels);
  free(data);
} TRACE_END


// -------------------------------------------------------------------------- //
// data_swap
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, data_swap, (Data* l, Data* r)) {
  Data tmp;

  tmp = *l;
  *l = *r;
  *r = tmp;
} TRACE_END

