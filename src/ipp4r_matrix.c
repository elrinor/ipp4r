#include <assert.h>
#include "ipp4r.h"
#include "ipp4r_matrix.h"


Matrix* matrix_new(VALUE r_matrix, int isMask) {
  Matrix* result;
  int r, c;
  int i, j;

  r = R2C_INT(rb_funcall(r_matrix, rb_ID_row_size, 0));
  c = R2C_INT(rb_funcall(r_matrix, rb_ID_column_size, 0));

  result = (Matrix*) malloc(sizeof(Matrix));
  result->size.height = r;
  result->size.width = c;
  result->isMask = isMask;
  result->data = malloc(r * c * (isMask ? sizeof(char) : sizeof(float)));

  for(i = 0; i < r; i++) {
    for(j = 0; j < c; j++) {
      VALUE v = rb_funcall_rescue(r_matrix, rb_ID_aref, 2, C2R_INT(i), C2R_INT(j));
      if(v == Qundef) {
        matrix_destroy(result);
        return NULL;
      }
      if(isMask)
        *((char*) result->data + i * c + j) = R2C_INT(v) > 0 ? 1 : 0;
      else
        *((float*) result->data + i * c + j) = R2C_FLT(v);
    }
  }

  return result;
}

void matrix_destroy(Matrix* c_matrix) {
  assert(c_matrix != NULL);
  assert(c_matrix->data != NULL);
  free(c_matrix->data);
  free(c_matrix);
}
