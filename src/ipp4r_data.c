#include <ipp.h>
#include "ipp4r.h"

// -------------------------------------------------------------------------- //
// data_new
// -------------------------------------------------------------------------- //
Data* data_new(int width, int height, IppChannels channels) {
  int wStep;
  void* pixels;
  Data* imagedata;

  imagedata = malloc(sizeof(Data)); // TODO: use ALLOC / xfree

  if(imagedata != NULL) {
    IPPMETACALL(channels, pixels, ippiMalloc_8u_, ARX_EMPTY, (3, (C1, C3, AC4)), (width, height, &wStep), NULL); // TODO: use xmalloc / xfree
    if(pixels != NULL) {
      imagedata->channels = channels;
      imagedata->height = height;
      imagedata->width = width;
      imagedata->wStep = wStep;
      imagedata->pixels = pixels;
    } else {
      free(imagedata);
      imagedata = NULL;
    }
  }
  return imagedata;
}


// -------------------------------------------------------------------------- //
// data_destroy
// -------------------------------------------------------------------------- //
void data_destroy(Data* imagedata) {
  ippiFree(imagedata->pixels);
  free(imagedata);
}
