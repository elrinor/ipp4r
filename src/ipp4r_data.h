#ifndef __IPP4R_IMAGEDATA_H__
#define __IPP4R_IMAGEDATA_H__

#include <ippdefs.h>
#include "ipp4r_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Data stores real image parameters, not the ROI ones, like Image does.
 * One instance of Data is shared between several instances of Image.
 */
struct _Data {
  IppChannels channels; /**< channel type of an image */

  void* pixels;         /**< pointer to aligned image data */
  int width;            /**< image width in pixels */
  int height;           /**< image height in pixels */
  int wStep;            /**< size of aligned image row in bytes */
  int pixelSize;        /**< size of one image pixel in bytes */

  int dx, dy;           /**< image growth values */
};


/**
 * Allocates memory for Data of given size.
 * 
 * @returns newly allocated Data, or NULL in case of an error.
 */
Data* data_new(int width, int height, IppChannels channels);


/**
 * Frees memory occupied by Data structure.
 */
void data_destroy(Data* data);


/**
 * Swaps fields of two given Data structures.
 */
void data_swap(Data* l, Data* r);


#ifdef __cplusplus
}
#endif

#endif