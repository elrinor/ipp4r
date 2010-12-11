#include <cassert>
#include "ipp4r.h"


// -------------------------------------------------------------------------- //
// Local functions
// -------------------------------------------------------------------------- //
/**
 * The purpose of this function is to check the metatype infrastructure.
 * If it compiles, then everything is OK.
 */
static void metatype_static_check(void) {
  /* check M_INDEX */
#define CHECK_M_INDEX(INDEX, ARG) STATIC_ASSERT(( M_INDEX(ARX_ARRAY_ELEM(INDEX, M_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(M_SUPPORTED)), CHECK_M_INDEX, ~)

  /* check D_INDEX */
#define CHECK_D_INDEX(INDEX, ARG) STATIC_ASSERT(( D_INDEX(ARX_ARRAY_ELEM(INDEX, D_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(D_SUPPORTED)), CHECK_D_INDEX, ~)

  /* check C_INDEX */
#define CHECK_C_INDEX(INDEX, ARG) STATIC_ASSERT(( C_INDEX(ARX_ARRAY_ELEM(INDEX, C_SUPPORTED)) == INDEX ));
  ARX_ARRAY_FOREACH(ARX_INDEX_ARRAY(ARX_ARRAY_SIZE(C_SUPPORTED)), CHECK_C_INDEX, ~)

  /* check accessors */
#define CHECK_M_ACCESSORS(METATYPE, ARG) STATIC_ASSERT(( M_CENUM(M_CREATE(M_DATATYPE(METATYPE), M_CHANNELS(METATYPE))) == M_CENUM(METATYPE) ));
  ARX_ARRAY_FOREACH(M_SUPPORTED, CHECK_M_ACCESSORS, ~)
}


// -------------------------------------------------------------------------- //
// ippmetatype_channels
// -------------------------------------------------------------------------- //
IppChannels metatype_channels(IppMetaType metaType) {
#define METAFUNC(M, ARGS) C_CENUM(M_CHANNELS(M))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
}


/**
* @returns IppDataType encoded in the given IppMetaType
*/
IppDataType metatype_datatype(IppMetaType metaType) {
#define METAFUNC(M, ARGS) D_CENUM(M_DATATYPE(M))
  IPPMETACALL(metaType, return, M_SUPPORTED, METAFUNC, ~, Unreachable(), -1);
#undef METAFUNC
}
