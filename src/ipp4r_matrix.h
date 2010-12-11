#ifndef __IPP4R_MATRIX_H__
#define __IPP4R_MATRIX_H__

#include <ruby.h>
#include "ipp4r_fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _Matrix {
  int isMask;       /**< char or float? */
  void* data;       /**< data */
  IppiSize size;    /**< size */
};

Matrix* matrix_new(VALUE r_matrix, int isMask);

void matrix_destroy(Matrix* c_matrix);

#ifdef __cplusplus
}
#endif

#endif