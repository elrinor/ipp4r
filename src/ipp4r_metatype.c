#include "ipp4r.h"

/**
 * The purpose of this function is to check the metatype infrastructure.
 * If it compiles, then everything is OK.
 */
static void metatype_static_check(void) {
  /* check M_INDEX macro */
#define CHECK_M_INDEX(INDEX, ARG) STATIC_ASSERT(( M_INDEX(ARX_ARRAY_ELEM(INDEX, SUPPORTED_METATYPES)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(SUPPORTED_METATYPES)), CHECK_M_INDEX, ~)

  /* check accessors */
#define CHECK_M_ACCESSORS(METATYPE, ARG) STATIC_ASSERT(( ARX_JOIN(ipp, ARX_JOIN(M_DATATYPE(METATYPE), ARX_JOIN(_, M_CHANNELS(METATYPE)))) == ARX_JOIN(ipp, METATYPE) ));
  ARX_ARRAY_FOREACH(SUPPORTED_METATYPES, CHECK_M_ACCESSORS, ~)
}