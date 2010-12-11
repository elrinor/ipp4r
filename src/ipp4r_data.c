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

  *wStep = (width * pixel_size(metaType) + 32 - 1) & -32;

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
Data* data_new(int width, int height, IppMetaType metaType) {
  int wStep;
  Data* data;
  void* pixels;

  assert(width > 0 && height > 0);

  data = ALLOC(Data); // ALLOC always succeeds or throws an exception
  pixels = ipp4rMalloc(width, height, metaType, &wStep); 
  
  if(pixels != NULL) {
    data->pixels = pixels;
    data->metaType = metaType;
    data->height = height;
    data->width = width;
    data->wStep = wStep;
    data->pixelSize = pixel_size(metaType);
  } else {
    xfree(data);
    data = NULL;
  }

  return data;
}


// -------------------------------------------------------------------------- //
// data_destroy
// -------------------------------------------------------------------------- //
void data_destroy(Data* data) {
  assert(data != NULL);

  ipp4rFree(data->pixels);
  xfree(data);
}


// -------------------------------------------------------------------------- //
// data_swap
// -------------------------------------------------------------------------- //
void data_swap(Data* l, Data* r) {
  Data tmp;

  tmp = *l;
  *l = *r;
  *r = tmp;
}

