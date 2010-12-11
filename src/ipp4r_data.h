#ifndef __IPP4R_IMAGEDATA_H__
#define __IPP4R_IMAGEDATA_H__

#include <ippdefs.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * IppImageData stores real image parameters, not the ROI ones, like IppImage does.
 * One instance of IppImageData is shared between several instances of IppImage.
 */
typedef struct _Data {
  IppChannels channels; /**< channel type of an image */

  void* pixels;         /**< pointer to aligned image data */
  int width;            /**< image width in pixels */
  int height;           /**< image height in pixels */
  int wStep;            /**< size of aligned image row in bytes */
} Data;


/**
 * Allocates memory for Data of given size.
 *  
 * @returns newly allocated Data, or NULL in case of an error.
 */
Data* data_new(int width, int height, IppChannels channels);


/**
 * Frees memory occupied by Data structure.
 */
void data_destroy(Data* imagedata);


#ifdef __cplusplus
}
#endif

#endif