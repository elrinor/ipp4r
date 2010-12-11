#include <assert.h>
#include <ipp.h>
#include <ruby.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// Debug facilities
// -------------------------------------------------------------------------- //
IF_TRACE(static int data_count = 0;)


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
 * Allocates a memory block aligned to a 32-byte boundary. <br>
 * Much like the one from ipp, but uses xmalloc provided by ruby. <br>
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
TRACE_FUNC(Data*, data_new, (int width, int height, IppMetaType metaType, int border)) {
  int wStep;
  Data* data;
  void* buffer;

  assert(width > 0 && height > 0 && border >= 0);

  /* We use malloc and not ALLOC here because:
   * 1. We don't want to mess with exception handling - our function mustn't throw.
   * 2. Data structure is small, therefore ruby GC won't benefit from knowing about it. */
  data = (Data*) malloc(sizeof(Data)); 
  if(data == NULL)
    TRACE_RETURN(NULL);

  buffer = ipp4rMalloc(width + 2 * border, height + 2 * border, metaType, &wStep); 
  if(buffer == NULL) {
    free(data);
    TRACE_RETURN(NULL);
  }

  data->buffer = buffer;
  data->metaType = metaType;
  data->height = height;
  data->width = width;
  data->wStep = wStep;
  data->pixelSize = metatype_pixel_size(metaType);
  data->border = border;

  data->shared = FALSE; /* Initially data is not shared */

  data->pixels = (char*) data->buffer + data->border * (data->wStep + data->pixelSize); /* pixels points to the "beginning" of an image */

  IF_TRACE(data_count++;)
  TRACE(("data_count=%d", data_count));
  TRACE(("%08X w=%d h=%d b=%d m=%d buf=%08X", data, width, height, border, metaType, buffer));

  TRACE_RETURN(data);
} TRACE_END


// -------------------------------------------------------------------------- //
// data_destroy
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, data_destroy, (Data* data)) {
  assert(data != NULL);

  IF_TRACE(data_count--;)
  TRACE(("data_count=%d", data_count));
  TRACE(("%08X w=%d h=%d m=%d buf=%08X", data, data->width, data->height, data->metaType, data->buffer));

  ipp4rFree(data->buffer);
  free(data);
} TRACE_END


// -------------------------------------------------------------------------- //
// data_swap
// -------------------------------------------------------------------------- //
TRACE_FUNC(void, data_swap, (Data* l, Data* r)) {
  Data tmp;
  int intTmp;

  tmp = *l;
  *l = *r;
  *r = tmp;

  /* We mustn't swap "shared" field, because shared determines whether the current data structure is registered in ruby gc.
   * Swapping data fields does not register / unregister a data structure. */
  intTmp = l->shared;
  l->shared = r->shared;
  r->shared = intTmp;
} TRACE_END





